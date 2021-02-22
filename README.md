# zynqFC
Xilinx Zynq SoC based all-in-one quadrocopter flight controller. Stereo 720p side-by-side digital video transmission.


> STILL UNDER DEVELOPMENT!




<b>Hardware</b>
- PCB with ~ 54 x 46 mm size and 30.5 x 30.5 mm mount hole distance
- Xilinx Zynq SoC Processor (Dual-Core ARM Cortex-A9 MPCore up to 866 MHz + Artix 7 FPGA fabric)
- MPU-6000 as Gyro
- BMX160 as IMU/Mag
- BMP280 barometer
- 16 MiByte flash and microSD slot
- 256 MiByte DDR3 RAM
- I²C EEPROM
- JTAG debugger interface
- nrFL24L01+ 2.4 GHz Transceiver with SMA antenna connector
- 2x camera connector for stereo video stream (MIPI CSI-2)
- AD9361 SDR RF transceiver chipset (5.8 GHz, 2 x 2 MIMO) to stream digital stereo video with 2x SMA antenna connector
- 4x BLDC motor connection jumpers (DShot600 protocol)
- 1x GPS module connector

<b>FPGA</b>
- DShot protocol peripheral
- MIPI CSI-2 receivers for video streams
- Video compression (H.264)
- Stereo OSD overlay
- Dynamic video data rate adjust (resize, sub-sampling, reduced color space) depending on video signal strength
- VTX modulation with Coded Orthogonal Frequency Division Multiplex (COFDM) (same as used in DVB-T)

<b>Software</b>
- Flight controller on ARM Cortex-A9 with 8 kHz looptime
- Minimum function: acro-mode, level-mode
- Boot from flash, microSD or JTAG (development)



