# Get location of this script
set rootPath [file dirname [file normalize [info script]]]
cd $rootPath

# Export Hardware Definition File and generate SDK Workspace in _sw folder
launch_sdk -workspace ../../_sw -hwspec zynqAIO.sdk/toplevel.hdf