# Get location of this script
set rootPath [file dirname [file normalize [info script]]]
cd $rootPath

# Do the same as Export Hardware in Vivado GUI (local to project, includes bitstream)
file mkdir zynqAIO.sdk
file copy -force zynqAIO.runs/impl_1/toplevel.sysdef zynqAIO.sdk/toplevel.hdf

# Export Hardware Definition File and generate SDK Workspace in _sw folder
launch_sdk -workspace ../../_sw -hwspec zynqAIO.sdk/toplevel.hdf