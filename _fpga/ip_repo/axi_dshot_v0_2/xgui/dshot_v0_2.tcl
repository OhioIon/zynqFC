# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  ipgui::add_page $IPINST -name "Page 0"


}

proc update_PARAM_VALUE.C_AXIL_ACLK_FREQ_HZ { PARAM_VALUE.C_AXIL_ACLK_FREQ_HZ } {
	# Procedure called to update C_AXIL_ACLK_FREQ_HZ when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_AXIL_ACLK_FREQ_HZ { PARAM_VALUE.C_AXIL_ACLK_FREQ_HZ } {
	# Procedure called to validate C_AXIL_ACLK_FREQ_HZ
	return true
}

proc update_PARAM_VALUE.C_AXIL_DATA_WIDTH { PARAM_VALUE.C_AXIL_DATA_WIDTH } {
	# Procedure called to update C_AXIL_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_AXIL_DATA_WIDTH { PARAM_VALUE.C_AXIL_DATA_WIDTH } {
	# Procedure called to validate C_AXIL_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.C_AXIL_ADDR_WIDTH { PARAM_VALUE.C_AXIL_ADDR_WIDTH } {
	# Procedure called to update C_AXIL_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_AXIL_ADDR_WIDTH { PARAM_VALUE.C_AXIL_ADDR_WIDTH } {
	# Procedure called to validate C_AXIL_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.C_AXIL_BASEADDR { PARAM_VALUE.C_AXIL_BASEADDR } {
	# Procedure called to update C_AXIL_BASEADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_AXIL_BASEADDR { PARAM_VALUE.C_AXIL_BASEADDR } {
	# Procedure called to validate C_AXIL_BASEADDR
	return true
}

proc update_PARAM_VALUE.C_AXIL_HIGHADDR { PARAM_VALUE.C_AXIL_HIGHADDR } {
	# Procedure called to update C_AXIL_HIGHADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_AXIL_HIGHADDR { PARAM_VALUE.C_AXIL_HIGHADDR } {
	# Procedure called to validate C_AXIL_HIGHADDR
	return true
}


proc update_MODELPARAM_VALUE.C_AXIL_DATA_WIDTH { MODELPARAM_VALUE.C_AXIL_DATA_WIDTH PARAM_VALUE.C_AXIL_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_AXIL_DATA_WIDTH}] ${MODELPARAM_VALUE.C_AXIL_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_AXIL_ADDR_WIDTH { MODELPARAM_VALUE.C_AXIL_ADDR_WIDTH PARAM_VALUE.C_AXIL_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_AXIL_ADDR_WIDTH}] ${MODELPARAM_VALUE.C_AXIL_ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.C_AXIL_ACLK_FREQ_HZ { MODELPARAM_VALUE.C_AXIL_ACLK_FREQ_HZ PARAM_VALUE.C_AXIL_ACLK_FREQ_HZ } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_AXIL_ACLK_FREQ_HZ}] ${MODELPARAM_VALUE.C_AXIL_ACLK_FREQ_HZ}
}

