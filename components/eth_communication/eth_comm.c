#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_event.h"
#include "driver/gpio.h"
#include "eth_comm.h"

void eth_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
	uint8_t mac_addr[6] = {0};
	// can get the ethernet driver handle from event data
	esp_eth_handle_t eth_handle = *(esp_eth_handle_t *)event_data;

	switch (event_id) {
	case ETHERNET_EVENT_CONNECTED:
		esp_eth_ioctl(eth_handle, ETH_CMD_G_MAC_ADDR, mac_addr);
		ESP_LOGI(ETH_TAG, "Ethernet Link Up");
		ESP_LOGI(ETH_TAG, "Ethernet HW Addr %02x:%02x:%02x:%02x:%02x:%02x",
				 mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
		break;
	case ETHERNET_EVENT_DISCONNECTED:
		ESP_LOGI(ETH_TAG, "Ethernet Link Down");
		break;
	case ETHERNET_EVENT_START:
		ESP_LOGI(ETH_TAG, "Ethernet Started");
		break;
	case ETHERNET_EVENT_STOP:
		ESP_LOGI(ETH_TAG, "Ethernet Stopped");
		break;
	default:
		break;
	}
}

void got_ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
	ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
	const esp_netif_ip_info_t *ip_info = &event->ip_info;

	ESP_LOGI(ETH_TAG, "Ethernet Got IP Address");
	ESP_LOGI(ETH_TAG, "~~~~~~~~~~~");
	ESP_LOGI(ETH_TAG, "ETHIP:" IPSTR, IP2STR(&ip_info->ip));
	ESP_LOGI(ETH_TAG, "ETHMASK:" IPSTR, IP2STR(&ip_info->netmask));
	ESP_LOGI(ETH_TAG, "ETHGW:" IPSTR, IP2STR(&ip_info->gw));
	ESP_LOGI(ETH_TAG, "~~~~~~~~~~~");
}

void eth_connect(void) // TODO: return esp_err_t
{
	// Create default event loop that running in background
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	// Register Ethernet event handler (to deal with user specific stuffs when event like link up/down happened)
	ESP_ERROR_CHECK(esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler, NULL));

	/* Connect Driver to TCP/IP Stack */
	// Initialize TCP/IP network interface (should be called only once in application)
	ESP_ERROR_CHECK(esp_netif_init());
	esp_netif_config_t netif_cfg = ESP_NETIF_DEFAULT_ETH(); // apply default network interface configuration for Ethernet
	esp_netif_t *eth_netif = esp_netif_new(&netif_cfg); // create network interface for Ethernet driver
	// Set default handlers to process TCP/IP stuffs
	ESP_ERROR_CHECK(esp_eth_set_default_handlers(eth_netif));
	// Register user defined IP event handler
	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler, NULL));

	/* Create MAC and PHY Instance */
	eth_mac_config_t eth_mac_config = ETH_MAC_DEFAULT_CONFIG(); // apply default MAC configuration
	eth_mac_config.smi_mdc_gpio_num = ETH_MDC_GPIO;
	eth_mac_config.smi_mdio_gpio_num = ETH_MDIO_GPIO;
	esp_eth_mac_t *eth_mac = esp_eth_mac_new_esp32(&eth_mac_config); // create MAC instance

	eth_phy_config_t eth_phy_config = ETH_PHY_DEFAULT_CONFIG(); // apply default PHY configuration
	eth_phy_config.phy_addr = ETH_PHY_ADDR;
	eth_phy_config.reset_gpio_num = ETH_PHY_RST_GPIO;
	// Olimex ESP32-PoE configuration
	gpio_pad_select_gpio(ETH_PHY_POWER_PIN);
	gpio_set_direction(ETH_PHY_POWER_PIN,GPIO_MODE_OUTPUT);
	gpio_set_level(ETH_PHY_POWER_PIN, 1);
	esp_eth_phy_t *eth_phy = esp_eth_phy_new_lan8720(&eth_phy_config); // create PHY instance

	/* Install Ethernet driver */
	esp_eth_config_t eth_config = ETH_DEFAULT_CONFIG(eth_mac, eth_phy); // apply default driver configuration
	esp_eth_handle_t eth_handle = NULL; // after driver installed, we will get the handle of the driver
	ESP_ERROR_CHECK(esp_eth_driver_install(&eth_config, &eth_handle)); // install driver

	/* Attach Ethernet driver to TCP/IP stack */
	ESP_ERROR_CHECK(esp_netif_attach(eth_netif, esp_eth_new_netif_glue(eth_handle)));

	/* Start Ethernet driver state machine */
	ESP_ERROR_CHECK(esp_eth_start(eth_handle));
}