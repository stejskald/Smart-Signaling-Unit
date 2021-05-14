set(srcs
    "led_strip_display_ctrl.cpp"
	"led_strip/led_strip_rmt_ws2812.c"
	"rgb565_rgb888_conv/rgb565_rgb888_conversions.c"
	"display_controller/display_controller.cpp")

set(include_dirs
	"include"
	"led_strip/include"
	"rgb565_rgb888_conv/include"
	"display_controller/include")

set(priv_requires driver)

idf_component_register(SRCS "${srcs}"
					INCLUDE_DIRS "${include_dirs}"
					PRIV_REQUIRES "${priv_requires}")