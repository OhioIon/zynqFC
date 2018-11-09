library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- dshot test-bench
entity dshot_tb is
end dshot_tb;

architecture Behavioral of dshot_tb is

  component dshot is
  generic (
    -- Parameters of Axi Slave Bus Interface AXIL
    C_AXIL_ACLK_FREQ_HZ  : integer  := 100000000;
    C_AXIL_DATA_WIDTH    : integer  := 32;
    C_AXIL_ADDR_WIDTH    : integer  := 5
  );
  port (
    -- DShot protocol output signal
    dshot_out    : out std_logic;
    -- Ports of Axi Slave Bus Interface AXIL
    axil_aclk    : in std_logic;
    axil_aresetn : in std_logic;
    axil_awaddr  : in std_logic_vector(C_AXIL_ADDR_WIDTH-1 downto 0);
    axil_awvalid : in std_logic;
    axil_awready : out std_logic;
    axil_wdata   : in std_logic_vector(C_AXIL_DATA_WIDTH-1 downto 0);
    axil_wstrb   : in std_logic_vector((C_AXIL_DATA_WIDTH/8)-1 downto 0);
    axil_wvalid  : in std_logic;
    axil_wready  : out std_logic;
    axil_bresp   : out std_logic_vector(1 downto 0);
    axil_bvalid  : out std_logic;
    axil_bready  : in std_logic;
    axil_araddr  : in std_logic_vector(C_AXIL_ADDR_WIDTH-1 downto 0);
    axil_arvalid : in std_logic;
    axil_arready : out std_logic;
    axil_rdata   : out std_logic_vector(C_AXIL_DATA_WIDTH-1 downto 0);
    axil_rresp   : out std_logic_vector(1 downto 0);
    axil_rvalid  : out std_logic;
    axil_rready  : in std_logic
  );
  end component;

  -- UUT interface
  signal dshot_out         :  std_logic;
  signal axil_aclk_in      :  std_logic := '1';
  signal axil_aresetn_in   :  std_logic := '0';
  signal axil_awaddr_in    :  std_logic_vector(31 downto 0) := (others=>'0');
  signal axil_awvalid_in   :  std_logic := '0';
  signal axil_awready_out  :  std_logic;
  signal axil_wdata_in     :  std_logic_vector(31 downto 0) := (others=>'0');
  signal axil_wstrb_in     :  std_logic_vector(3 downto 0) := (others=>'0');
  signal axil_wvalid_in    :  std_logic := '0';
  signal axil_wready_out   :  std_logic;
  signal axil_bresp_out    :  std_logic_vector(1 downto 0);
  signal axil_bvalid_out   :  std_logic;
  signal axil_bready_in    :  std_logic := '0';
  signal axil_araddr_in    :  std_logic_vector(31 downto 0) := (others=>'0');
  signal axil_arvalid_in   :  std_logic := '0';
  signal axil_arready_out  :  std_logic;
  signal axil_rdata_out    :  std_logic_vector(31 downto 0);
  signal axil_rresp_out    :  std_logic_vector(1 downto 0);
  signal axil_rvalid_out   :  std_logic;
  signal axil_rready_in    :  std_logic := '0';
  
  -- Reference Output
  signal dshot_out_ref     : std_logic := '0';

begin

-- Instantiate UUT and assign signals
uut : dshot
generic map (
  C_AXIL_ACLK_FREQ_HZ => 100000000,
  C_AXIL_DATA_WIDTH   => 32,
  C_AXIL_ADDR_WIDTH   => 32
)
port map (
  dshot_out    => dshot_out        ,
  axil_aclk    => axil_aclk_in     ,
  axil_aresetn => axil_aresetn_in  ,
  axil_awaddr  => axil_awaddr_in   ,
  axil_awvalid => axil_awvalid_in  ,
  axil_awready => axil_awready_out ,
  axil_wdata   => axil_wdata_in    ,
  axil_wstrb   => axil_wstrb_in    ,
  axil_wvalid  => axil_wvalid_in   ,
  axil_wready  => axil_wready_out  ,
  axil_bresp   => axil_bresp_out   ,
  axil_bvalid  => axil_bvalid_out  ,
  axil_bready  => axil_bready_in   ,
  axil_araddr  => axil_araddr_in   ,
  axil_arvalid => axil_arvalid_in  ,
  axil_arready => axil_arready_out ,
  axil_rdata   => axil_rdata_out   ,
  axil_rresp   => axil_rresp_out   ,
  axil_rvalid  => axil_rvalid_out  ,
  axil_rready  => axil_rready_in   
);

-- Create clock and reset signal
axil_aclk_in    <= not axil_aclk_in after 2.5 ns;
axil_aresetn_in <= '1' after 20 ns;

-- Simulate AXI4-Lite write access
axil_bready_in <= '1';
process 
begin
  wait for 40 ns;
  axil_awaddr_in <= x"00000008"; axil_awvalid_in <= '1'; axil_wdata_in <= x"000000A7"; axil_wstrb_in <= x"F"; axil_wvalid_in <= '1'; wait for 10 ns; -- cntBitTot = 167
  axil_awaddr_in <= x"0000000C"; axil_awvalid_in <= '1'; axil_wdata_in <= x"0000003E"; axil_wstrb_in <= x"F"; axil_wvalid_in <= '1'; wait for 10 ns; -- cntT0H    = 62
  axil_awaddr_in <= x"00000010"; axil_awvalid_in <= '1'; axil_wdata_in <= x"0000007D"; axil_wstrb_in <= x"F"; axil_wvalid_in <= '1'; wait for 10 ns; -- cntT1H    = 125
  axil_awaddr_in <= x"00000000"; axil_awvalid_in <= '0'; axil_wdata_in <= x"00000000"; axil_wstrb_in <= x"0"; axil_wvalid_in <= '0'; wait for 10 ns;
  axil_awaddr_in <= x"00000000"; axil_awvalid_in <= '1'; axil_wdata_in <= x"00000793"; axil_wstrb_in <= x"F"; axil_wvalid_in <= '1'; wait for 10 ns; -- data = 1939, tel = 0, START!
  axil_awaddr_in <= x"00000000"; axil_awvalid_in <= '0'; axil_wdata_in <= x"00000000"; axil_wstrb_in <= x"0"; axil_wvalid_in <= '0'; wait for 10 ns;
  wait for 15 us;
  axil_awaddr_in <= x"00000000"; axil_awvalid_in <= '1'; axil_wdata_in <= x"80000050"; axil_wstrb_in <= x"F"; axil_wvalid_in <= '1'; wait for 10 ns; -- data = 80, tel = 1, START!
  axil_awaddr_in <= x"00000000"; axil_awvalid_in <= '0'; axil_wdata_in <= x"00000000"; axil_wstrb_in <= x"0"; axil_wvalid_in <= '0'; wait for 10 ns;
  wait;
end process;

-- Generate reference signal according to DShot1200 timing
process
begin
  wait for 115 ns;
  
  dshot_out_ref <= '1'; wait for 625 ns; -- T1H
  dshot_out_ref <= '0'; wait for 210 ns; -- T1L  
  
  dshot_out_ref <= '1'; wait for 625 ns; -- T1H
  dshot_out_ref <= '0'; wait for 210 ns; -- T1L  
  
  dshot_out_ref <= '1'; wait for 625 ns; -- T1H
  dshot_out_ref <= '0'; wait for 210 ns; -- T1L  
  
  dshot_out_ref <= '1'; wait for 625 ns; -- T1H
  dshot_out_ref <= '0'; wait for 210 ns; -- T1L  
  
  dshot_out_ref <= '1'; wait for 312.5 ns; -- T0H
  dshot_out_ref <= '0'; wait for 522.5 ns; -- T0L  
  
  dshot_out_ref <= '1'; wait for 312.5 ns; -- T0H
  dshot_out_ref <= '0'; wait for 522.5 ns; -- T0L
  
  dshot_out_ref <= '1'; wait for 625 ns; -- T1H
  dshot_out_ref <= '0'; wait for 210 ns; -- T1L  
    
  dshot_out_ref <= '1'; wait for 312.5 ns; -- T0H
  dshot_out_ref <= '0'; wait for 522.5 ns; -- T0L  
  
  dshot_out_ref <= '1'; wait for 312.5 ns; -- T0H
  dshot_out_ref <= '0'; wait for 522.5 ns; -- T0L
   
  dshot_out_ref <= '1'; wait for 625 ns; -- T1H
  dshot_out_ref <= '0'; wait for 210 ns; -- T1L  
  
  dshot_out_ref <= '1'; wait for 625 ns; -- T1H
  dshot_out_ref <= '0'; wait for 210 ns; -- T1L  
    
  dshot_out_ref <= '1'; wait for 312.5 ns; -- T0H
  dshot_out_ref <= '0'; wait for 522.5 ns; -- T0L
    
  dshot_out_ref <= '1'; wait for 625 ns; -- T1H
  dshot_out_ref <= '0'; wait for 210 ns; -- T1L  
    
  dshot_out_ref <= '1'; wait for 312.5 ns; -- T0H
  dshot_out_ref <= '0'; wait for 522.5 ns; -- T0L
  
  dshot_out_ref <= '1'; wait for 625 ns; -- T1H
  dshot_out_ref <= '0'; wait for 210 ns; -- T1L  
 
  dshot_out_ref <= '1'; wait for 625 ns; -- T1H
  dshot_out_ref <= '0'; wait for 210 ns; -- T1L
  
  wait for 1.66 us;
  
  dshot_out_ref <= '1'; wait for 312.5 ns; -- T0H
  dshot_out_ref <= '0'; wait for 522.5 ns; -- T0L  
    
  dshot_out_ref <= '1'; wait for 312.5 ns; -- T0H
  dshot_out_ref <= '0'; wait for 522.5 ns; -- T0L  
      
  dshot_out_ref <= '1'; wait for 312.5 ns; -- T0H
  dshot_out_ref <= '0'; wait for 522.5 ns; -- T0L  
      
  dshot_out_ref <= '1'; wait for 312.5 ns; -- T0H
  dshot_out_ref <= '0'; wait for 522.5 ns; -- T0L  
    
  dshot_out_ref <= '1'; wait for 625 ns; -- T1H
  dshot_out_ref <= '0'; wait for 210 ns; -- T1L  
  
  dshot_out_ref <= '1'; wait for 312.5 ns; -- T0H
  dshot_out_ref <= '0'; wait for 522.5 ns; -- T0L  
    
  dshot_out_ref <= '1'; wait for 625 ns; -- T1H
  dshot_out_ref <= '0'; wait for 210 ns; -- T1L  
  
  dshot_out_ref <= '1'; wait for 312.5 ns; -- T0H
  dshot_out_ref <= '0'; wait for 522.5 ns; -- T0L  
    
  dshot_out_ref <= '1'; wait for 312.5 ns; -- T0H
  dshot_out_ref <= '0'; wait for 522.5 ns; -- T0L  
      
  dshot_out_ref <= '1'; wait for 312.5 ns; -- T0H
  dshot_out_ref <= '0'; wait for 522.5 ns; -- T0L  
      
  dshot_out_ref <= '1'; wait for 312.5 ns; -- T0H
  dshot_out_ref <= '0'; wait for 522.5 ns; -- T0L  
        
  dshot_out_ref <= '1'; wait for 625 ns; -- T1H
  dshot_out_ref <= '0'; wait for 210 ns; -- T1L  
    
  dshot_out_ref <= '1'; wait for 625 ns; -- T1H
  dshot_out_ref <= '0'; wait for 210 ns; -- T1L  
    
  dshot_out_ref <= '1'; wait for 312.5 ns; -- T0H
  dshot_out_ref <= '0'; wait for 522.5 ns; -- T0L
  
  dshot_out_ref <= '1'; wait for 625 ns; -- T1H
  dshot_out_ref <= '0'; wait for 210 ns; -- T1L  
  
  dshot_out_ref <= '1'; wait for 625 ns; -- T1H
  dshot_out_ref <= '0'; wait for 210 ns; -- T1L

  wait;
   
end process;

end Behavioral;
