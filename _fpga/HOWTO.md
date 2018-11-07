/// Recreate vivado project after fresh git clone ///
- start Xilinx Vivado 2018.2 GUI
- menu "Tools" -> "Run Tcl Script..."
- select the "import.tcl" located in the vivado folder
- this will use a project .tcl script file to recreate and open the project

/// Save changes in vivado project for git commit/push ///
- Open the Vivado project
- Make sure the block-design has no locked IP cores, all IP cores are up to date, and "validate design" is run
- menu "Tools" -> "Run Tcl Script..."
- select the "export.tcl" located in this folder
- this will generate and overwrite the project .tcl file in the vivado folder by using the "write_project_tcl" command