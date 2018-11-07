# Get project directory path
set prjDir [get_property DIRECTORY [current_project]]

# Export main.bd.tcl which can be used to recreate the "main" block design in Vivado
cd $prjDir/../bd/main
open_bd_design main.bd
write_bd_tcl -force -include_layout main.bd.tcl
close_bd_design main

# Export zynqAIO.xpr.tcl which can be used to recreate the project in Vivado
cd $prjDir
write_project_tcl -force -all_properties -no_copy_sources -use_bd_files -target_proj_dir . $prjDir/zynqAIO.xpr.tcl