library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity toplevel is
  port (
    -- PS DDR3
    DDR_addr : inout STD_LOGIC_VECTOR ( 14 downto 0 );
    DDR_ba : inout STD_LOGIC_VECTOR ( 2 downto 0 );
    DDR_cas_n : inout STD_LOGIC;
    DDR_ck_n : inout STD_LOGIC;
    DDR_ck_p : inout STD_LOGIC;
    DDR_cke : inout STD_LOGIC;
    DDR_cs_n : inout STD_LOGIC;
    DDR_dm : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_dq : inout STD_LOGIC_VECTOR ( 31 downto 0 );
    DDR_dqs_n : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_dqs_p : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_odt : inout STD_LOGIC;
    DDR_ras_n : inout STD_LOGIC;
    DDR_reset_n : inout STD_LOGIC;
    DDR_we_n : inout STD_LOGIC;
    -- PS MIO
    FIXED_IO_ddr_vrn : inout STD_LOGIC;
    FIXED_IO_ddr_vrp : inout STD_LOGIC;
    FIXED_IO_mio : inout STD_LOGIC_VECTOR ( 53 downto 0 );
    FIXED_IO_ps_clk : inout STD_LOGIC;
    FIXED_IO_ps_porb : inout STD_LOGIC;
    FIXED_IO_ps_srstb : inout STD_LOGIC;
    -- DShot
    dshot_0  : out STD_LOGIC;
    dshot_1  : out STD_LOGIC;
    dshot_2  : out STD_LOGIC;
    dshot_3  : out STD_LOGIC;
    -- EEPROM IIC
    eeprom_iic_scl : inout STD_LOGIC;
    eeprom_iic_sda : inout STD_LOGIC;
    -- IMU IIC
    imu_iic_scl : inout STD_LOGIC;
    imu_iic_sda : inout STD_LOGIC;
    imu_int     : inout STD_LOGIC
  );
end toplevel;

architecture STRUCTURE of toplevel is

  component main is
  port (
    -- PS DDR3
    DDR_cas_n : inout STD_LOGIC;
    DDR_cke : inout STD_LOGIC;
    DDR_ck_n : inout STD_LOGIC;
    DDR_ck_p : inout STD_LOGIC;
    DDR_cs_n : inout STD_LOGIC;
    DDR_reset_n : inout STD_LOGIC;
    DDR_odt : inout STD_LOGIC;
    DDR_ras_n : inout STD_LOGIC;
    DDR_we_n : inout STD_LOGIC;
    DDR_ba : inout STD_LOGIC_VECTOR ( 2 downto 0 );
    DDR_addr : inout STD_LOGIC_VECTOR ( 14 downto 0 );
    DDR_dm : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_dq : inout STD_LOGIC_VECTOR ( 31 downto 0 );
    DDR_dqs_n : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_dqs_p : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    -- PS MIO
    FIXED_IO_mio : inout STD_LOGIC_VECTOR ( 53 downto 0 );
    FIXED_IO_ddr_vrn : inout STD_LOGIC;
    FIXED_IO_ddr_vrp : inout STD_LOGIC;
    FIXED_IO_ps_srstb : inout STD_LOGIC;
    FIXED_IO_ps_clk : inout STD_LOGIC;
    FIXED_IO_ps_porb : inout STD_LOGIC;
    -- PS GPIO EMIO
    GPIO_EMIO_tri_i : in STD_LOGIC_VECTOR ( 63 downto 0 );
    GPIO_EMIO_tri_o : out STD_LOGIC_VECTOR ( 63 downto 0 );
    GPIO_EMIO_tri_t : out STD_LOGIC_VECTOR ( 63 downto 0 );
    -- DShot (motors/ESCs)
    dshot_0 : out STD_LOGIC;
    dshot_1 : out STD_LOGIC;
    dshot_2 : out STD_LOGIC;
    dshot_3 : out STD_LOGIC;
    -- EEPROM
    iic_eeprom_scl_i : in STD_LOGIC;
    iic_eeprom_scl_o : out STD_LOGIC;
    iic_eeprom_scl_t : out STD_LOGIC;
    iic_eeprom_sda_i : in STD_LOGIC;
    iic_eeprom_sda_o : out STD_LOGIC;
    iic_eeprom_sda_t : out STD_LOGIC;
    -- IIC IMU (MPU 6000)
    iic_imu_scl_i : in STD_LOGIC;
    iic_imu_scl_o : out STD_LOGIC;
    iic_imu_scl_t : out STD_LOGIC;
    iic_imu_sda_i : in STD_LOGIC;
    iic_imu_sda_o : out STD_LOGIC;
    iic_imu_sda_t : out STD_LOGIC
  );
  end component main;
  
  component IOBUF is
  port (
    I : in STD_LOGIC;
    O : out STD_LOGIC;
    T : in STD_LOGIC;
    IO : inout STD_LOGIC
  );
  end component IOBUF;
  
  -- GPIO EMIO
  signal gpio_emio_i : STD_LOGIC_VECTOR ( 117 downto 54 );
  signal gpio_emio_o : STD_LOGIC_VECTOR ( 117 downto 54 );
  signal gpio_emio_t : STD_LOGIC_VECTOR ( 117 downto 54 );
    
  -- EEPROM IIC
  signal iic_eeprom_scl_i : STD_LOGIC;
  signal iic_eeprom_scl_o : STD_LOGIC;
  signal iic_eeprom_scl_t : STD_LOGIC;
  signal iic_eeprom_sda_i : STD_LOGIC;
  signal iic_eeprom_sda_o : STD_LOGIC;
  signal iic_eeprom_sda_t : STD_LOGIC;
  
  -- IMU IIC
  signal iic_imu_scl_i : STD_LOGIC;
  signal iic_imu_scl_o : STD_LOGIC;
  signal iic_imu_scl_t : STD_LOGIC;
  signal iic_imu_sda_i : STD_LOGIC;
  signal iic_imu_sda_o : STD_LOGIC;
  signal iic_imu_sda_t : STD_LOGIC;

-----------------------------------------------------------------------------
begin
-----------------------------------------------------------------------------

-- Main Block Diagram
main_i: component main
port map (
  -- PS DDR3
  DDR_addr(14 downto 0) => DDR_addr(14 downto 0),
  DDR_ba(2 downto 0) => DDR_ba(2 downto 0),
  DDR_cas_n => DDR_cas_n,
  DDR_ck_n => DDR_ck_n,
  DDR_ck_p => DDR_ck_p,
  DDR_cke => DDR_cke,
  DDR_cs_n => DDR_cs_n,
  DDR_dm(3 downto 0) => DDR_dm(3 downto 0),
  DDR_dq(31 downto 0) => DDR_dq(31 downto 0),
  DDR_dqs_n(3 downto 0) => DDR_dqs_n(3 downto 0),
  DDR_dqs_p(3 downto 0) => DDR_dqs_p(3 downto 0),
  DDR_odt => DDR_odt,
  DDR_ras_n => DDR_ras_n,
  DDR_reset_n => DDR_reset_n,
  DDR_we_n => DDR_we_n,
  -- PS MIO
  FIXED_IO_ddr_vrn => FIXED_IO_ddr_vrn,
  FIXED_IO_ddr_vrp => FIXED_IO_ddr_vrp,
  FIXED_IO_mio(53 downto 0) => FIXED_IO_mio(53 downto 0),
  FIXED_IO_ps_clk => FIXED_IO_ps_clk,
  FIXED_IO_ps_porb => FIXED_IO_ps_porb,
  FIXED_IO_ps_srstb => FIXED_IO_ps_srstb,
  -- GPIO EMIO
  GPIO_EMIO_tri_i => gpio_emio_i,
  GPIO_EMIO_tri_o => gpio_emio_o,
  GPIO_EMIO_tri_t => gpio_emio_t,
  -- DShot
  dshot_0 => dshot_0,
  dshot_1 => dshot_1,
  dshot_2 => dshot_2,
  dshot_3 => dshot_3,
    -- IMU IIC
  iic_eeprom_scl_i => iic_eeprom_scl_i,
  iic_eeprom_scl_o => iic_eeprom_scl_o,
  iic_eeprom_scl_t => iic_eeprom_scl_t,
  iic_eeprom_sda_i => iic_eeprom_sda_i,
  iic_eeprom_sda_o => iic_eeprom_sda_o,
  iic_eeprom_sda_t => iic_eeprom_sda_t,  
  -- IMU IIC
  iic_imu_scl_i => iic_imu_scl_i,
  iic_imu_scl_o => iic_imu_scl_o,
  iic_imu_scl_t => iic_imu_scl_t,
  iic_imu_sda_i => iic_imu_sda_i,
  iic_imu_sda_o => iic_imu_sda_o,
  iic_imu_sda_t => iic_imu_sda_t
);
 
-- EEPROM IIC 
iic_eeprom_scl_iobuf: component IOBUF
port map (
  I  => iic_eeprom_scl_o,
  IO => eeprom_iic_scl,
  O  => iic_eeprom_scl_i,
  T  => iic_eeprom_scl_t
);

iic_eeprom_sda_iobuf: component IOBUF
port map (
  I  => iic_eeprom_sda_o,
  IO => eeprom_iic_sda,
  O  => iic_eeprom_sda_i,
  T  => iic_eeprom_sda_t
);
    
-- IMU IIC 
iic_imu_scl_iobuf: component IOBUF
port map (
  I  => iic_imu_scl_o,
  IO => imu_iic_scl,
  O  => iic_imu_scl_i,
  T  => iic_imu_scl_t
);

iic_imu_sda_iobuf: component IOBUF
port map (
  I  => iic_imu_sda_o,
  IO => imu_iic_sda,
  O  => iic_imu_sda_i,
  T  => iic_imu_sda_t
);

iic_imu_int_iobuf: component IOBUF
port map (
  I  => gpio_emio_o(54),
  IO => imu_int,
  O  => gpio_emio_i(54),
  T  => gpio_emio_t(54)
);
    
end STRUCTURE;

-- EOF
