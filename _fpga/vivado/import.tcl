# Get location of this script and change working directory here (import.tcl)
set dispScriptFile [file normalize [info script]]
set scriptFolder [file dirname $dispScriptFile]
cd $scriptFolder

# Execute the project.tcl to create project
source project.tcl