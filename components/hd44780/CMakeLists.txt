idf_component_register(
	SRCS "hd44780.cpp" "i2c-interface.cpp"
	INCLUDE_DIRS "include"
	REQUIRES "error" "i2c_wrapper" 
)