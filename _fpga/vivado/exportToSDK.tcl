# Get location of this script
set rootPath [file dirname [file normalize [info script]]]
cd $rootPath

# Do the same as Export Hardware in Vivado GUI (local to project, includes bitstream)
file mkdir zynqFC.sdk
file copy -force zynqFC.runs/impl_1/toplevel.sysdef zynqFC.sdk/toplevel.hdf

# Export Hardware Definition File and generate SDK Workspace in _sw folder
launch_sdk -workspace ../../_sw -hwspec zynqFC.sdk/toplevel.hdf