/// Create vivado project after git clone ///
- start Vivado GUI
- menu "Tools" -> "Run Tcl Script..."
- select the "import.tcl" located in this folder
- this will use the project.tcl script file to create the project in this folder

/// Save changes in vivado project for git commit/push ///
- menu "Tools" -> "Run Tcl Script..."
- select the "export.tcl" located in this folder
- this will generate and overwrite the project.tcl file in this folder by using the vivado "write_project_tcl" command