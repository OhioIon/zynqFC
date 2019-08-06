# DShot
set_property PACKAGE_PIN N15 [get_ports dshot_0]; # Zybo XADC-PMOD JA1_P
set_property PACKAGE_PIN L14 [get_ports dshot_1]; # Zybo XADC-PMOD JA2_P
set_property PACKAGE_PIN K16 [get_ports dshot_2]; # Zybo XADC-PMOD JA3_P
set_property PACKAGE_PIN K14 [get_ports dshot_3]; # Zybo XADC-PMOD JA4_P
set_property IOSTANDARD LVCMOS33 [get_ports dshot_0];
set_property IOSTANDARD LVCMOS33 [get_ports dshot_1];
set_property IOSTANDARD LVCMOS33 [get_ports dshot_2];
set_property IOSTANDARD LVCMOS33 [get_ports dshot_3];

# IIC EEPROM
set_property PACKAGE_PIN N18 [get_ports iic_eeprom_scl]; # Zybo on-board
set_property PACKAGE_PIN N17 [get_ports iic_eeprom_sda]; # Zybo on-board
set_property IOSTANDARD LVCMOS33 [get_ports iic_eeprom_scl];
set_property IOSTANDARD LVCMOS33 [get_ports iic_eeprom_sda];

# IIC IMU
set_property PACKAGE_PIN W20 [get_ports iic_imu_scl]; # Zybo PMOD JB2_N
set_property PACKAGE_PIN V20 [get_ports iic_imu_sda]; # Zybo PMOD JB2_P
set_property PACKAGE_PIN W19 [get_ports iic_imu_int]; # Zybo PMOD JB4_N
set_property IOSTANDARD LVCMOS33 [get_ports iic_imu_scl];
set_property IOSTANDARD LVCMOS33 [get_ports iic_imu_sda];
set_property IOSTANDARD LVCMOS33 [get_ports iic_imu_int];
