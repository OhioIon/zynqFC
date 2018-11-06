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
    Data10,
    Data9,
    Data8,
    Data7,
    Data6,
    Data5,
    Data4,
    Data3,
    Data2,
    Data1,
    Data0,
    ReqTele,
    Crc3,
    Crc2,
    Crc1,
    Crc0
  );

  signal bitCount_u16    : unsigned(15 downto 0);        -- Bit-Time Counter
  signal bit_u16         : unsigned(15 downto 0);        -- High-Time for Current Bit
  signal i_cntBitTot_u16 : unsigned(15 downto 0);        -- Tick Count for Total Bit
  signal i_cntT0H_u16    : unsigned(15 downto 0);        -- Tick Count for Zero Bit Value
  signal i_cntT1H_u16    : unsigned(15 downto 0);        -- Tick Count for One Bit Value
  signal crc_u4          : std_logic_vector(3 downto 0); -- CRC
  signal stMac           : stateMachine;                 -- Waveform state machine

begin

  -- Determine CRC by XOR of data bits
  crc_u4 <= (dshotData_u11(2 downto 0) & flgReqTele_l) XOR dshotData_u11(6 downto 3) XOR dshotData_u11(10 downto 7);
  
  -- Convert inputs to unsigned
  i_cntBitTot_u16 <= Unsigned(cntBitTot_u16);
  i_cntT0H_u16    <= Unsigned(cntT0H_u16);
  i_cntT1H_u16    <= Unsigned(cntT1H_u16);
	
	process( aclk ) 
	begin
    if rising_edge(aclk) then 
      if aresetn = '0' then
        -- Reset Signals
        bitCount_u16 <= x"0000";
        stMac        <= Idle;
        -- Reset Outputs
        flgBusy_l    <= '0';
        dshot_l      <= '0';
      else
      
        case stMac is
                
          when Idle =>
            if( flgStart_l = '1' ) then
              flgBusy_l    <= '1';
              dshot_l      <= '1';
              stMac        <= Data10;
              bitCount_u16 <= x"0001";
              if ( dshotData_u11(10) = '1' ) then
                bit_u16 <= i_cntT1H_u16;
              else 
                bit_u16 <= i_cntT0H_u16;
              end if;
            end if;
            
          when Data10 =>
            if( bitCount_u16 < bit_u16 ) then
              dshot_l      <= '1';
              bitCount_u16 <= bitCount_u16 + 1;
            elsif( bitCount_u16 < i_cntBitTot_u16 ) then
              dshot_l      <= '0';
              bitCount_u16 <= bitCount_u16 + 1;
            else
              dshot_l      <= '1';
              stMac        <= Data9;
              bitCount_u16 <= x"0001";
              if ( dshotData_u11(9) = '1' ) then
                bit_u16 <= i_cntT1H_u16;
              else 
                bit_u16 <= i_cntT0H_u16;
              end if;
            end if;  
                      
          when Data9 =>
            if( bitCount_u16 < bit_u16 ) then
              dshot_l      <= '1';
              bitCount_u16 <= bitCount_u16 + 1;
            elsif( bitCount_u16 < i_cntBitTot_u16 ) then
              dshot_l      <= '0';
              bitCount_u16 <= bitCount_u16 + 1;
            else
              dshot_l      <= '1';
              stMac        <= Data8;
              bitCount_u16 <= x"0001";
              if ( dshotData_u11(8) = '1' ) then
                bit_u16 <= i_cntT1H_u16;
              else 
                bit_u16 <= i_cntT0H_u16;
              end if;
            end if;
          
          when others =>
          
        end case;

      end if;
    end if;
	end process;

end arch_imp;
