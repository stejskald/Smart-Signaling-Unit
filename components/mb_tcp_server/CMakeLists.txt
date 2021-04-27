set(srcs
	"mb_tcp_server.c"
	"modbus_params/modbus_params.c")

set(include_dirs
	"include"
	"modbus_params/include")

set(priv_requires
	eth_comm
	nvs_flash
	freemodbus)

idf_component_register(SRCS "${srcs}"
					INCLUDE_DIRS "${include_dirs}"
					PRIV_REQUIRES "${priv_requires}")
