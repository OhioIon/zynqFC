library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity dshot is
  generic (
    -- Parameters of Axi Slave Bus Interface AXIL
    C_AXIL_ACLK_FREQ_HZ  : integer  := 100000000;
    C_AXIL_DATA_WIDTH    : integer  := 32;
    C_AXIL_ADDR_WIDTH    : integer  := 5
  );
  port (
    -- DShot protocol output signal
    dshot_out      : out std_logic;
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
end dshot;

architecture arch_imp of dshot is

  -- sub-component declaration
  component dshot_AXIL is
    generic (
    C_S_AXI_DATA_WIDTH  : integer  := 32;
    C_S_AXI_ADDR_WIDTH  : integer  := 5
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
    reg0_out : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg1_out : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg2_out : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg3_out : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg4_out : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg5_out : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg6_out : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg7_out : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg0_in  : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg1_in  : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg2_in  : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg3_in  : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg4_in  : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg5_in  : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg6_in  : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg7_in  : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    reg0_WR  : out std_logic;
    reg1_WR  : out std_logic;
    reg2_WR  : out std_logic;
    reg3_WR  : out std_logic;
    reg4_WR  : out std_logic;
    reg5_WR  : out std_logic;
    reg6_WR  : out std_logic;
    reg7_WR  : out std_logic
    );
  end component dshot_AXIL;
  
  component dshot_stMac is
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
  end component dshot_stMac;
  
  -- Memory map feedback
  signal reg0              : std_logic_vector(31 downto 0);
  signal reg1              : std_logic_vector(31 downto 0);
  signal reg2              : std_logic_vector(31 downto 0);
  signal reg3              : std_logic_vector(31 downto 0);
  signal reg4              : std_logic_vector(31 downto 0);
  signal reg7              : std_logic_vector(31 downto 0);
  
  -- From CPU
  signal dshotData_u11     : std_logic_vector(10 downto 0);
  signal flgReqTele_l      : std_logic;
  signal flgStart_l        : std_logic;
  signal cntBitTot_u16     : std_logic_vector(15 downto 0);
  signal cntT0H_u16        : std_logic_vector(15 downto 0);
  signal cntT1H_u16        : std_logic_vector(15 downto 0);
  
  -- To CPU
  signal flgBusy_l         : std_logic;
  
  -- DShot output signal FFs
  signal dshot_to_ff       : std_logic;
  signal dshot_ff          : std_logic;

begin

-- Instantiation of Axi Bus Interface AXIL
dshot_AXIL_inst : dshot_AXIL
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
  reg0_WR           => flgStart_l, -- Writing to reg0 starts DShot transmission

  -- RO
  reg1_out          => open, 
  reg1_in           => reg1,
  reg1_WR           => open,

  -- RW
  reg2_out          => reg2,
  reg2_in           => reg2,
  reg2_WR           => open,
  
  -- RW
  reg3_out          => reg3,
  reg3_in           => reg3,
  reg3_WR           => open,
  
  -- RW
  reg4_out          => reg4,
  reg4_in           => reg4,
  reg4_WR           => open,
  
  -- Unused
  reg5_out          => open,
  reg5_in           => (others => '0'),
  reg5_WR           => open,
  
  -- Unused
  reg6_out          => open,
  reg6_in           => (others => '0'),
  reg6_WR           => open,
  
  -- RO
  reg7_out          => open,
  reg7_in           => reg7,
  reg7_WR           => open

);
  
-- From CPU
dshotData_u11 <= reg0(10 downto  0);
flgReqTele_l  <= reg0(31);
cntBitTot_u16 <= reg2(15 downto 0);
cntT0H_u16    <= reg3(15 downto 0);
cntT1H_u16    <= reg4(15 downto 0);

-- To CPU
reg1(0)       <= flgBusy_l;
reg7          <= std_logic_vector(to_unsigned(C_AXIL_ACLK_FREQ_HZ,32)); -- Allow driver software to automatically obtain clock frequency
  
  
-- Instantiation of DShot waveform generation state machine
dshot_stMac_inst : dshot_stMac
port map (
  -- Global
  aclk          => axil_aclk,     -- Clock
  aresetn       => axil_aresetn,  -- nReset
  -- Inputs
  dshotData_u11 => dshotData_u11, -- DShot Data
  flgReqTele_l  => flgReqTele_l,  -- Request Telemetry Bit
  flgStart_l    => flgStart_l,    -- Start Transmission Trigger
  -- Parameters
  cntBitTot_u16 => cntBitTot_u16, -- Number of clock ticks for complete bit
  cntT0H_u16    => cntT0H_u16,    -- Number of clock ticks the output is high for bit value zero
  cntT1H_u16    => cntT1H_u16,    -- Number of clock ticks the output is high for bit value one
  -- Outputs
  flgBusy_l     => flgBusy_l,     -- Protocol transmission busy
  dshot_l       => dshot_to_ff    -- DShot data signal (to ESC)
);

-- Use one flip-flop to provide ease-of-routing towards output buffer/pad
-- Use a second Flip-Flop that can be placed directly into the output buffer/pad for reproducible output timings
process(axil_aclk)
begin
  if rising_edge(axil_aclk) then 
    if axil_aresetn = '0' then
      dshot_ff  <= '0';
      dshot_out <= '0';
    else
      dshot_ff  <= dshot_to_ff; -- 1st FF
      dshot_out <= dshot_ff;    -- 2nd FF
    end if;
  end if;
end process;


end arch_imp;
