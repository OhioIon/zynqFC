# Get location of this script and change working directory here (import.tcl)
set dispScriptFile [file normalize [info script]]
set prjDir [file dirname $dispScriptFile]


# Execute the .tcl script to create the main block design
cd $prjDir/../bd/main/
source main.bd.tcl

# Execute the .tcl script to create the Vivado project
cd $prjDir
source zynqAIO.xpr.tcl