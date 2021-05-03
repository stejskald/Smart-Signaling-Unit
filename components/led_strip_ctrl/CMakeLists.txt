set(srcs
    "led_strip_ctrl.cpp"
	"led_strip/led_strip_rmt_ws2812.c")

set(include_dirs
	"include"
	"led_strip/include")

set(priv_requires driver)

idf_component_register(SRCS "${srcs}"
					INCLUDE_DIRS "${include_dirs}"
					PRIV_REQUIRES "${priv_requires}")