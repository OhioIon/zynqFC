/// Recreate vivado project after fresh git clone ///
- start Xilinx Vivado 2018.2 GUI
- menu "Tools" -> "Run Tcl Script..."
- select the "import.tcl" located in the vivado folder
- this will use a project .tcl script file to recreate and open the project

/// Generate FPGA bitstream file and export HDF to Xilinx SDK ///
- In the zynqAIO.xpr Vivado project, click on "Generate Bitstream"
- It takes a while to generate the FPGA bitstream here vivado/zynqAIO.runs/impl_1/toplevel.bit
- The bitfile is the FPGA "software" that needs to be loaded into the FPGA part of the Zynq on every boot
- Once complete select File -> Export -> Export Hardware
- Check "Include bitstream" and hit OK
- This will generate the hardware definition file vivado/zynqAIO.sdk/toplevel.hdf
- This file loaded into a SDK "Hardware Platform Specification" project
- It is basically a .zip file containing some auto-coded sources for configuration of the Zynq during boot

/// Save changes in vivado project for git commit/push ///
- Open the Vivado project
- Make sure the block-design has no locked IP cores, all IP cores are up to date, and "validate design" is run
- menu "Tools" -> "Run Tcl Script..."
- select the "export.tcl" located in the vivado folder
- this will generate and overwrite the project .tcl file in the vivado folder by using the "write_project_tcl" command