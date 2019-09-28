/// Recreate vivado project after fresh git clone ///
- start Xilinx Vivado 2018.2 GUI
- menu "Tools" -> "Run Tcl Script..."
- select the "import.tcl" located in the vivado folder
  - this will use a project .tcl script file to recreate and open the project
  - also the main block-design in the project is recreated

/// Generate FPGA bitstream file and export HDF to Xilinx SDK ///
- in the zynqFC.xpr Vivado project, click on "Generate Bitstream"
  - it takes a while to generate the FPGA bitstream (toplevel.bit)
  - the bitfile is the FPGA "software" that needs to be loaded into the FPGA part of the Zynq on every boot
- once complete select "Tools" -> "Run Tcl Script..."
- Hint: If SDK is already open it should be closed before running the tcl script!
- select the "exportToSDK.tcl" located in the vivado folder
  - this will use the hardware definition file toplevel.sysdef
  - it is automatically created from Vivado during build and includes the bitfile as well (it's basically a .zip folder)
  - a SDK workspace is created, SDK is launched, and a "Hardware Platform Specification" project is created based on this file
  - In case the workspace/project already exist, they are merly updated
  - Continue with the HOWTO.md in the _sw folder...

/// Save changes in vivado project for git commit/push ///
- open the Vivado project
- make sure the block-design has no locked IP cores, all IP cores are up to date, and "validate design" is run
- menu "Tools" -> "Run Tcl Script..."
- select the "export.tcl" located in the vivado folder
  - this will generate and overwrite the project .tcl file in the vivado folder by using the "write_project_tcl" command
