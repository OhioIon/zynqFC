# Get location of this script
set rootPath [file dirname [file normalize [info script]]]

# Execute the .tcl script to create the main block design
cd $rootPath
source bd/main/main.bd.tcl

# Execute the .tcl script to create the Vivado project
cd $rootPath/vivado
source zynqAIO.xpr.tcl