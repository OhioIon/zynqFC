# Get location of this script
set rootPath [file dirname [file normalize [info script]]]

# Execute the .tcl script to create the Vivado project and block design file
cd $rootPath
source zynqAIO.xpr.tcl