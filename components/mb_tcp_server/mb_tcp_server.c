// #include "sdkconfig.h"		// for KConfig options
#include "mb_tcp_server.h"	// for Modbus TCP Server implementation
#include "esp_err.h"		// for error handling

void nvs_flash_initialization(void){
	esp_err_t result = nvs_flash_init(); // NVS ... Non-volatile Storage
	if (result == ESP_ERR_NVS_NO_FREE_PAGES || result == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		result = nvs_flash_init();
	}
	ESP_ERROR_CHECK(result);
	esp_netif_init();
	ESP_ERROR_CHECK(esp_event_loop_create_default());
}

