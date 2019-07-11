# Get location of this script
set rootPath [file dirname [file normalize [info script]]]
cd $rootPath

# Execute the .tcl script to create the Vivado project and block design file
source zynqFC.xpr.tcl