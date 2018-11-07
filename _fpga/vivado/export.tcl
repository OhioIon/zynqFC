# Get project directory path
set prjDir [get_property DIRECTORY [current_project]]

# Export project.tcl which can be used to recreate the project in Vivado
write_project_tcl -force -all_properties -target_proj_dir . $prjDir/project.tcl