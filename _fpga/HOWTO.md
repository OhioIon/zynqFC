/// Create vivado project after git clone (do once) ///
- start Vivado GUI
- menu "Tools" -> "Run Tcl Script..."
- select the "generate.tcl" located in this folder
- this will use a project .tcl script file to recreate the project in the vivado folder
- the block design .tcl in the bd folder is used to recreate the block design


/// Save changes in vivado project for git commit/push ///
- menu "Tools" -> "Run Tcl Script..."
- select the "export.tcl" located in this folder
- this will generate and overwrite the project .tcl file in the vivado folder by using the vivado "write_project_tcl" command
- this will generate and overwrite the block design 