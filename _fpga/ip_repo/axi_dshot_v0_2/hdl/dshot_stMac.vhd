library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity dshot_stMac is
  port (
  
    -- Global
    aclk          : in  std_logic;                     -- Clock
    aresetn       : in  std_logic;                     -- nReset
  
    -- Inputs
    dshotData_u11 : in  std_logic_vector(10 downto 0); -- DShot Data
    flgReqTele_l  : in  std_logic;                     -- Request Telemetry Bit
    flgStart_l    : in  std_logic;                     -- Start Transmission Trigger
    
    -- Parameters
    cntBitTot_u16 : in  std_logic_vector(15 downto 0); -- Number of clock ticks for complete bit
    cntT0H_u16    : in  std_logic_vector(15 downto 0); -- Number of clock ticks the output is high for bit value zero
    cntT1H_u16    : in  std_logic_vector(15 downto 0); -- Number of clock ticks the output is high for bit value one
    
    -- Outputs
    flgBusy_l     : out std_logic;                     -- Protocol transmission busy
    dshot_l       : out std_logic                      -- DShot data signal (to ESC)
    
  );
end dshot_stMac;

architecture arch_imp of dshot_stMac is

  type stateMachine is (
    Idle,
    Delay,
    Data,
    ReqTele,
    Crc
  );

  -- Bit Wavform Geneartion
  signal bitCount_u16    : unsigned(15 downto 0) := (others=>'0'); -- Bit-Time Counter
  signal bit_u16         : unsigned(15 downto 0) := (others=>'0'); -- High-Time for Current Bit
  signal i_cntBitTot_u16 : unsigned(15 downto 0) := (others=>'0'); -- Tick Count for Total Bit
  signal i_cntT0H_u16    : unsigned(15 downto 0) := (others=>'0'); -- Tick Count for Zero Bit Value
  signal i_cntT1H_u16    : unsigned(15 downto 0) := (others=>'0'); -- Tick Count for One Bit Value
  signal flgEnaCntr_l    : std_logic := '0';                       -- Enable Counter
  signal dshot0_l        : std_logic := '0';                       -- DShot waveform for 0 bit value
  signal dshot1_l        : std_logic := '0';                       -- DShot waveform for 1 bit value
  
  -- State Machine
  signal crc_u4          : std_logic_vector(3 downto 0) := x"0";   -- CRC
  signal stMac           : stateMachine                 := Idle;   -- Waveform state machine
  signal idx_u4          : unsigned(3 downto 0)         := x"0";   -- Bit index for current bit             

begin

  -- Determine CRC by XOR of data bits
  crc_u4 <= (dshotData_u11(2 downto 0) & flgReqTele_l) XOR dshotData_u11(6 downto 3) XOR dshotData_u11(10 downto 7);
  
  -- Convert inputs to unsigned
  i_cntBitTot_u16 <= Unsigned(cntBitTot_u16);
  i_cntT0H_u16    <= Unsigned(cntT0H_u16);
  i_cntT1H_u16    <= Unsigned(cntT1H_u16);
  
  -- Bit Waveform Generation
  process( aclk ) 
  begin
    if rising_edge(aclk) then 
      if flgEnaCntr_l = '0' then
        bitCount_u16 <= x"0001";
        dshot0_l     <= '0';
        dshot1_l     <= '0';
      else
        
        -- Count Bit Time
        if (bitCount_u16 < i_cntBitTot_u16) then
          bitCount_u16 <= bitCount_u16 + 1;
        else
          bitCount_u16 <= x"0001";
        end if;
              
        -- Bit 0 Waveform
        if (bitCount_u16 <= i_cntT0H_u16) then
          dshot0_l <= '1';
        else
          dshot0_l <= '0';
        end if;
        
        -- Bit 1 Waveform
        if (bitCount_u16 <= i_cntT1H_u16) then
          dshot1_l <= '1';
        else
          dshot1_l <= '0';
        end if;
        
      end if;
    end if;
  end process;
  
  -- Busy is singaled in case not idle
  flgBusy_l <= '0' when (stMac = Idle) else '1';
  
  -- State Machine
  process( aclk ) 
  begin
    if rising_edge(aclk) then 
      if aresetn = '0' then
        -- Reset Signals
        stMac        <= Idle;
        idx_u4       <= x"0";
        flgEnaCntr_l <= '0';
        -- Reset Outputs
        dshot_l      <= '0';
      else
      
        case stMac is
                
          when Idle =>
            if( flgStart_l = '1' ) then
              -- Start State Machine
              flgEnaCntr_l <= '1';
              stMac        <= Delay;
              dshot_l      <= '0';
            end if;
            
          when Delay =>
            stMac  <= Data;
            idx_u4 <= x"A";
            dshot_l <= '0';
            
          when Data =>
          
            -- Forward bit waveform for current data bit
            if (dshotData_u11(to_integer(idx_u4)) = '1') then
              dshot_l <= dshot1_l;
            else 
              dshot_l <= dshot0_l;
            end if;
          
            if( bitCount_u16 >= i_cntBitTot_u16 ) then
              -- Bit time has passed
              if (idx_u4 /= x"0") then
                -- There are still data bits
                idx_u4 <= idx_u4 - 1;
              else
                -- All data bits processed
                stMac <= ReqTele;
              end if;  
            end if;  
            
          when ReqTele =>
          
            -- Forward bit waveform for telemetry request bit
            if (flgReqTele_l = '1') then
              dshot_l <= dshot1_l;
            else 
              dshot_l <= dshot0_l;
            end if;
            
            if( bitCount_u16 >= i_cntBitTot_u16 ) then
              -- Bit time has passed
              idx_u4  <= x"3";
              stMac   <= Crc;
            end if;  
              
          when Crc =>
          
            -- Forward bit waveform for current CRC bit
            if ( crc_u4(to_integer(idx_u4)) = '1' ) then
              dshot_l <= dshot1_l;
            else 
              dshot_l <= dshot0_l;
            end if;
          
            if( bitCount_u16 >= i_cntBitTot_u16 ) then
              -- Bit time has passed
              if (idx_u4 /= x"0") then
                -- There are still CRC bits
                idx_u4 <= idx_u4 - 1;
              else
                -- All CRC bits processed
                stMac        <= Idle;
                idx_u4       <= x"0";
                dshot_l      <= '0';
                flgEnaCntr_l <= '0';
              end if;  
            end if;
            
        end case;

      end if;
    end if;
  end process;

end arch_imp;
