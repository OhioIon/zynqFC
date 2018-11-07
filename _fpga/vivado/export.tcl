# Get location of this script
set rootPath [file dirname [file normalize [info script]]]
cd $rootPath

# Export  project .tcl which can be used to recreate the project and block-design in Vivado
write_project_tcl -force -all_properties -target_proj_dir . zynqAIO.xpr.tcl