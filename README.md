# zynqAIO
Xilinx Zynq based all-in-one quadrocopter flight controller with digital stereo video stream for virtual reality headset flying

<b>Hardware</b>
- PCB with ~ 54 x 46 mm size and 30 x 30 mm mount hole distance
- Xilinx Zynq SoC Processor (Dual-Core ARM Cortex-A9 MPCore up to 866 MHz + Artix 7 FPGA fabric)
- IMU/Gyro MPU-6000 + mag sensors included
- 16 MiByte flash and microSD slot as boot options
- 256 MiByte DDR3 RAM
- I²C EEPROM
- JTAG debugger interface
- nrFL24L01+ 2.4 GHz Transceiver with SMA antenna connector
- 2x MIPI CSI-2 camera module socket (Raspberry PI camera module compatible connector)
- 2.4 GHz, 2 x 2 MIMO Wifi for 100 Mbps data rate with two SMA antenna connectors
- 4x BLDC motor connection jumpers (DShot600 protocol)

<b>Software</b>
- Flight controller can run either on MicroBlaze in FPGA or on ARM core 0 (driverless implementation)
- DShot protocol offload into FPGA hardware
- Access to gyro/imu via SPI peripheral
- Access to mag via I²C peripheral
- Access to 2.4 GHz nrFL24L01+ transciever via SPI
- JTAG debugging + JTAG UART for STDOUT
- MIPI CSI-2 camera signal decoding in FPGA fabric
- Access to camera module registers via I²C 
- MPEG codec in FPGA for single frame compression on both video streams
- On-the-fly resizing of video content depending on wifi signal strengt
- Side-By-Side transmission of stereo video content
- SDIO interface to wifi IC
