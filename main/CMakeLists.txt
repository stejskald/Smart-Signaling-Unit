# The component named main is special because it automatically requires all other components in the build.
# So it’s not necessary to pass REQUIRES or PRIV_REQUIRES to this component.
#
# See: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html#component-requirements
idf_component_register(SRCS "smart_signaling_unit.c"
                    INCLUDE_DIRS ".")
