set(srcs
    "display_driver.cpp"
	"led_strip/led_strip_rmt_ws2812.c"
	"rgb565_rgb888_conv/rgb565_rgb888_conversions.c")

set(include_dirs
	"include"
	"led_strip/include"
	"rgb565_rgb888_conv/include")

set(priv_requires driver)

idf_component_register(SRCS "${srcs}"
					INCLUDE_DIRS "${include_dirs}"
					PRIV_REQUIRES "${priv_requires}")