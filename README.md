# zynqAIO
Xilinx Zynq based all-in-one quadrocopter open-hardware open-software flight controller + camera + video receiver with digital stereo video stream for virtual reality headset flying

<h1>Flight Controller - zynqAIO_FC</h1>

<b>Hardware</b>
- PCB with ~ 54 x 46 mm size and 30.5 x 30.5 mm mount hole distance
- Xilinx Zynq SoC Processor (Dual-Core ARM Cortex-A9 MPCore up to 866 MHz + Artix 7 FPGA fabric)
- Bosch Sensortec BMX160 as IMU/Gyro/Mag
- 16 MiByte flash and microSD slot
- 256 MiByte DDR3 RAM
- I²C EEPROM
- JTAG debugger interface
- nrFL24L01+ 2.4 GHz Transceiver with SMA antenna connector
- 2x camera connector for stereo video stream (MIPI CSI-2)
- AD9361 RF transceiver chipset (2.4 GHz, 2 x 2 MIMO) to stream digital stereo video with 2x SMA antenna connector
- 4x BLDC motor connection jumpers (DShot1200 protocol)
- 1x GPS module connector

<b>FPGA</b>
- DShot protocol peripheral
- MIPI CSI-2 receiver
- Video compression (MJPEG/H.264/H.265/AV1 -> whatever is feasible in Zynq Artix fabric)
- Stereo OSD overlay
- Dynamic video data rate adjust (resize, sub-sampling, reduced color space) depending on VTX signal strength
- VTX modulation (LDPC, OFDM, IFFT)

<b>Software</b>
- Flight controller can run on MicroBlaze in FPGA or on ARM core 0 
- Use existing open-source FC -> TBD
- Minimum function: acro-mode, level-mode
- Boot from flash, microSD or JTAG

<h1>Stereo Video Board - zynqAIO_DUALCAM</h1>

<b>Hardware</b>
- CMOS camera board with dual OV5647 (1080p30, 720p60)
- 2x camera connector for stereo video stream (MIPI CSI-2)

<h1>Video Receiver Board - zynqAIO_VRX</h1>

<b>Hardware</b>
- Xilinx Zynq SoC Processor (Dual-Core ARM Cortex-A9 MPCore up to 866 MHz + Artix 7 FPGA fabric)
- AD9361 RF transceiver chipset (2.4 GHz, 2 x 2 MIMO) to receive digital stereo video with 2x SMA antenna connector
- nrFL24L01+ 2.4 GHz Transceiver with SMA antenna connector
- 16 MiByte flash and microSD slot
- 256 MiByte DDR3 RAM
- JTAG debugger interface
- I²C EEPROM
- 1x GPS module connector
- 2x USB
- 1x HDMI out for stereo image (FPV/VR googles)
- 1x HDMI out for mono image (screen, beamer)

<b>FPGA</b>
- VTX demodulation 
- Video decompression
- Video size adjust (upsampling to 1080p, regardless of input size)
- HDMI out

