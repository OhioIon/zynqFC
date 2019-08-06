##### DShot (to ESCs) #####
set_property PACKAGE_PIN N15 [get_ports dshot_0]; # Zybo XADC-PMOD JA1_P
set_property PACKAGE_PIN L14 [get_ports dshot_1]; # Zybo XADC-PMOD JA2_P
set_property PACKAGE_PIN K16 [get_ports dshot_2]; # Zybo XADC-PMOD JA3_P
set_property PACKAGE_PIN K14 [get_ports dshot_3]; # Zybo XADC-PMOD JA4_P
set_property IOSTANDARD LVCMOS33 [get_ports dshot_0];
set_property IOSTANDARD LVCMOS33 [get_ports dshot_1];
set_property IOSTANDARD LVCMOS33 [get_ports dshot_2];
set_property IOSTANDARD LVCMOS33 [get_ports dshot_3];

##### EEPROM (24AA02E48) #####
set_property PACKAGE_PIN N18 [get_ports eeprom_iic_scl]; # Zybo on-board
set_property PACKAGE_PIN N17 [get_ports eeprom_iic_sda]; # Zybo on-board
set_property IOSTANDARD LVCMOS33 [get_ports eeprom_iic_scl];
set_property IOSTANDARD LVCMOS33 [get_ports eeprom_iic_sda];

##### IMU (MPU-6000) #####
set_property PACKAGE_PIN W20 [get_ports imu_iic_scl]; # Zybo PMOD JB2_N
set_property PACKAGE_PIN V20 [get_ports imu_iic_sda]; # Zybo PMOD JB2_P
set_property PACKAGE_PIN W19 [get_ports imu_int];     # Zybo PMOD JB4_N
set_property IOSTANDARD LVCMOS33 [get_ports imu_iic_scl];
set_property IOSTANDARD LVCMOS33 [get_ports imu_iic_sda];
set_property IOSTANDARD LVCMOS33 [get_ports imu_int];

##### TRX (nRF24L01) #####
# E9 - TRX_SPI_MOSI - PS MIO10 - Zybo PMOD JF2
# C6 - TRX_SPI_MISO - PS MIO11 - Zybo PMOD JF3
# D9 - TRX_SPI_CLK  - PS MIO12 - Zybo PMOD JF4
# E8 - TRX_SPI_nCS  - PS MIO13 - Zybo PMOD JF1
# C5 - TRX_CE       - PS MIO14 - Zybo PMOD JF9
# C8 - TRX_IRQ      - PS MIO15 - Zybo PMOD JF10

# EOF
