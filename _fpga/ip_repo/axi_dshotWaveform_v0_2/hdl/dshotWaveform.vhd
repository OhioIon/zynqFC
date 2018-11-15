library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity dshotWaveform is
  generic (
    -- Parameters of Axi Slave Bus Interface AXIL
    C_AXIL_ACLK_FREQ_HZ  : integer  := 100000000;
    C_AXIL_DATA_WIDTH    : integer  := 32;
    C_AXIL_ADDR_WIDTH    : integer  := 4 -- 2^4 = 16 Byte address space = 4 registers
  );
  port (
    -- DShot Bit-Waveform in AXI4-Stream structure
    axis_tvalid    : out std_logic := '0'; -- Waveform data is valid
    axis_tlast     : out std_logic := '0'; -- Last clock cycle of waveform
    axis_tdata     : out std_logic_vector(7 downto 0) := (others=>'0'); -- High bit and low bit waveform signals (other bits unused)
    -- Ports of Axi Slave Bus Interface AXIL
    axil_aclk      : in std_logic;
    axil_aresetn   : in std_logic;
    axil_awaddr    : in std_logic_vector(C_AXIL_ADDR_WIDTH-1 downto 0);
    axil_awvalid   : in std_logic;
    axil_awready   : out std_logic;
    axil_wdata     : in std_logic_vector(C_AXIL_DATA_WIDTH-1 downto 0);
    axil_wstrb     : in std_logic_vector((C_AXIL_DATA_WIDTH/8)-1 downto 0);
    axil_wvalid    : in std_logic;
    axil_wready    : out std_logic;
    axil_bresp     : out std_logic_vector(1 downto 0);
    axil_bvalid    : out std_logic;
    axil_bready    : in  std_logic;
    axil_araddr    : in std_logic_vector(C_AXIL_ADDR_WIDTH-1 downto 0);
    axil_arvalid   : in std_logic;
    axil_arready   : out std_logic;
    axil_rdata     : out std_logic_vector(C_AXIL_DATA_WIDTH-1 downto 0);
    axil_rresp     : out std_logic_vector(1 downto 0);
    axil_rvalid    : out std_logic;
    axil_rready    : in  std_logic
  );
end dshotWaveform;

architecture arch_imp of dshotWaveform is

  -- sub-component declaration
  component dshotWaveform_AXIL is
    generic (
    C_S_AXI_DATA_WIDTH  : integer  := 32;
    C_S_AXI_ADDR_WIDTH  : integer  := 4
    );
    port (
    -- AXIL
    S_AXI_ACLK     : in  std_logic;
    S_AXI_ARESETN  : in  std_logic;
    S_AXI_AWADDR   : in  std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
    S_AXI_AWVALID  : in  std_logic;
    S_AXI_AWREADY  : out std_logic;
    S_AXI_WDATA    : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    S_AXI_WSTRB    : in  std_logic_vector((C_S_AXI_DATA_WIDTH/8)-1 downto 0);
    S_AXI_WVALID   : in  std_logic;
    S_AXI_WREADY   : out std_logic;
    S_AXI_BRESP    : out std_logic_vector(1 downto 0);
    S_AXI_BVALID   : out std_logic;
    S_AXI_BREADY   : in  std_logic;
    S_AXI_ARADDR   : in  std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
    S_AXI_ARVALID  : in  std_logic;
    S_AXI_ARREADY  : out std_logic;
    S_AXI_RDATA    : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    S_AXI_RRESP    : out std_logic_vector(1 downto 0);
    S_AXI_RVALID   : out std_logic;
    S_AXI_RREADY   : in  std_logic;
    -- RegIf
    reg0_out : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0) := (others=>'0');
    reg1_out : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0) := (others=>'0');
    reg2_out : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0) := (others=>'0');
    reg3_out : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0) := (others=>'0');
    reg0_in  : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg1_in  : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg2_in  : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg3_in  : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg0_WR  : out std_logic := '0';
    reg1_WR  : out std_logic := '0';
    reg2_WR  : out std_logic := '0';
    reg3_WR  : out std_logic := '0'
    );
  end component dshotWaveform_AXIL;
  
  -- Memory map
  signal reg0              : std_logic_vector(31 downto 0) := (others=>'0');
  signal reg1              : std_logic_vector(31 downto 0) := (others=>'0');
  signal reg2              : std_logic_vector(31 downto 0) := (others=>'0');
  
  -- From CPU
  signal flgStart_l        : std_logic := '0';
  signal cntBitTot_u16     : std_logic_vector(15 downto 0) := (others=>'0');
  signal cntT0H_u16        : std_logic_vector(15 downto 0) := (others=>'0');
  signal cntT1H_u16        : std_logic_vector(15 downto 0) := (others=>'0');
  
  -- DShot Bit Wavform Geneartion
  signal bitCount_u16    : unsigned(15 downto 0) := (others=>'0'); -- Bit-Time Counter
  signal i_cntBitTot_u16 : unsigned(15 downto 0) := (others=>'0'); -- Tick Count for Total Bit
  signal i_cntT0H_u16    : unsigned(15 downto 0) := (others=>'0'); -- Tick Count for Zero Bit Value
  signal i_cntT1H_u16    : unsigned(15 downto 0) := (others=>'0'); -- Tick Count for One Bit Value

begin

-- Instantiation of Axi Bus Interface AXIL
dshotWaveform_AXIL_inst : dshotWaveform_AXIL
generic map (
  C_S_AXI_DATA_WIDTH  => C_AXIL_DATA_WIDTH,
  C_S_AXI_ADDR_WIDTH  => C_AXIL_ADDR_WIDTH
)
port map (
  -- AXIL
  S_AXI_ACLK       => axil_aclk,
  S_AXI_ARESETN    => axil_aresetn,
  S_AXI_AWADDR     => axil_awaddr,
  S_AXI_AWVALID    => axil_awvalid,
  S_AXI_AWREADY    => axil_awready,
  S_AXI_WDATA      => axil_wdata,
  S_AXI_WSTRB      => axil_wstrb,
  S_AXI_WVALID     => axil_wvalid,
  S_AXI_WREADY     => axil_wready,
  S_AXI_BRESP      => axil_bresp,
  S_AXI_BVALID     => axil_bvalid,
  S_AXI_BREADY     => axil_bready,
  S_AXI_ARADDR     => axil_araddr,
  S_AXI_ARVALID    => axil_arvalid,
  S_AXI_ARREADY    => axil_arready,
  S_AXI_RDATA      => axil_rdata,
  S_AXI_RRESP      => axil_rresp,
  S_AXI_RVALID     => axil_rvalid,
  S_AXI_RREADY     => axil_rready,
  
  -- Register Interface
  
  -- RW
  reg0_out          => reg0, 
  reg0_in           => reg0,
  reg0_WR           => open,

  -- RW
  reg1_out          => reg1, 
  reg1_in           => reg1,
  reg1_WR           => open,
  
  -- RO
  reg2_out          => open, 
  reg2_in           => reg2,
  reg2_WR           => open,
  
  -- Unused
  reg3_out          => open, 
  reg3_in           => (others=>'0'),
  reg3_WR           => open

);
  
-- From CPU
flgStart_l    <= reg0(31);
cntBitTot_u16 <= reg0(15 downto  0);
cntT0H_u16    <= reg1(15 downto  0);
cntT1H_u16    <= reg1(31 downto 16);

-- To CPU
reg2(31 downto 0) <= std_logic_vector(to_unsigned(C_AXIL_ACLK_FREQ_HZ,32)); -- Allow driver software to automatically obtain clock frequency
  
-- Convert inputs to unsigned
i_cntBitTot_u16 <= Unsigned(cntBitTot_u16);
i_cntT0H_u16    <= Unsigned(cntT0H_u16);
i_cntT1H_u16    <= Unsigned(cntT1H_u16);

-- Unused TDATA bits
axis_tdata(7 downto 2) <= (others=>'0');


-- Bit Waveform Generation
process( axil_aclk ) 
begin
  if rising_edge(axil_aclk) then
    if flgStart_l = '0' then
      bitCount_u16  <= x"0000";
      axis_tvalid   <= '0';
      axis_tlast    <= '0';
      axis_tdata(0) <= '0';
      axis_tdata(1) <= '0';
    else
    
      -- Data is valid
      axis_tvalid <= '1';
       
      -- Count Bit Time
      if (bitCount_u16 < i_cntBitTot_u16) then
        bitCount_u16 <= bitCount_u16 + 1;
        axis_tlast <= '0';
      else
        axis_tlast <= '1';
        bitCount_u16 <= x"0000";
      end if;
      
      -- Bit 0 Waveform
      if (bitCount_u16 < i_cntT0H_u16) then
        axis_tdata(0) <= '1';
      else
        axis_tdata(0) <= '0';
      end if;
      
      -- Bit 1 Waveform
      if (bitCount_u16 < i_cntT1H_u16) then
        axis_tdata(1) <= '1';
      else
        axis_tdata(1) <= '0';
      end if;
      
    end if;
  end if;
end process;
 
end arch_imp;
