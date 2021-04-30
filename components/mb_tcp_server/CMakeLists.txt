set(srcs
	"mb_tcp_server.cpp"
	"modbus_params/modbus_params.cpp")

set(include_dirs
	"include"
	"modbus_params/include")

set(requires eth_comm)

set(priv_requires
	nvs_flash
	freemodbus)

idf_component_register(SRCS "${srcs}"
					INCLUDE_DIRS "${include_dirs}"
					REQUIRES "${requires}"
					PRIV_REQUIRES "${priv_requires}")
