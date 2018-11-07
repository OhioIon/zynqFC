# Get location of this script
set rootPath [file dirname [file normalize [info script]]]

# Export zynqAIO.xpr.tcl which can be used to recreate the project and bd in Vivado
cd $rootPath/vivado
write_project_tcl -force -all_properties -target_proj_dir . zynqAIO.xpr.tcl