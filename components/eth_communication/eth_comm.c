#include "sdkconfig.h"			// for KConfig options
#include "esp_log.h"			// for logging functionalities
#include "esp_event.h"			// for event handling
#include "driver/gpio.h"		// for gpio port number and port mode defines
#include "esp_eth.h"			// for Ethernet MAC and SPI-Ethernet support
#include "esp_netif_defaults.h"	// for netif assemble macros
#include "eth_comm.h"			// for establishing ethernet communication

#define OLIMEX_ESP32_POE_CONFIG
#ifdef OLIMEX_ESP32_POE_CONFIG
/**
 * @brief Ethernet Connection defines - Olimex ESP32-PoE configuration, same as for SSU
 * 
 */
#define ETH_MDC_GPIO		(23) // Output to PHY
#define ETH_MDIO_GPIO		(18) // Bidirectional
#define ETH_PHY_RST_GPIO	(-1)
#define ETH_PHY_ADDR		(0)
#define ETH_PHY_POWER_PIN	(12)
#endif

static const char *TAG = "eth";

void eth_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
	uint8_t mac_addr[6] = {0};
	// can get the ethernet driver handle from event data
	esp_eth_handle_t eth_handle = *(esp_eth_handle_t *)event_data;

	switch (event_id) {
	case ETHERNET_EVENT_CONNECTED:
		esp_eth_ioctl(eth_handle, ETH_CMD_G_MAC_ADDR, mac_addr);
		ESP_LOGI(TAG, "Ethernet Link Up");
		ESP_LOGI(TAG, "Ethernet HW Addr %02x:%02x:%02x:%02x:%02x:%02x",
				 mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
		break;
	case ETHERNET_EVENT_DISCONNECTED:
		ESP_LOGI(TAG, "Ethernet Link Down");
		break;
	case ETHERNET_EVENT_START:
		ESP_LOGI(TAG, "Ethernet Started");
		break;
	case ETHERNET_EVENT_STOP:
		ESP_LOGI(TAG, "Ethernet Stopped");
		break;
	default:
		break;
	}
}

static esp_netif_t *s_esp_netif = NULL;
static xSemaphoreHandle s_semph_get_ip_addr;

void got_ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
	ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
	const esp_netif_ip_info_t *ip_info = &event->ip_info;

	ESP_LOGI(TAG, "Ethernet Got IP Address");
	ESP_LOGI(TAG, "Connected to %s", esp_netif_get_desc(s_esp_netif));
	ESP_LOGI(TAG, "ETHIP:" IPSTR, IP2STR(&ip_info->ip));
	ESP_LOGI(TAG, "ETHMASK:" IPSTR, IP2STR(&ip_info->netmask));
	ESP_LOGI(TAG, "ETHGW:" IPSTR, IP2STR(&ip_info->gw));

	xSemaphoreGive(s_semph_get_ip_addr);
}

static esp_eth_handle_t s_eth_handle = NULL; // after driver installed, will get the handle of the driver
static esp_eth_mac_t *s_eth_mac = NULL;
static esp_eth_phy_t *s_eth_phy = NULL;
static void *s_eth_glue = NULL;

esp_netif_t *eth_start(void) {
	// TODO: Is this semaphore needed?
	s_semph_get_ip_addr = xSemaphoreCreateCounting(1, 0); // create semaphore if an interface is active

	// Create default event loop that running in background
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	// Register Ethernet event handler (to deal with user specific stuffs when event like link up/down happened)
	ESP_ERROR_CHECK(esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler, NULL));

	/* Connect Driver to TCP/IP Stack */
	// Initialize TCP/IP network interface (should be called only once in application)
	ESP_ERROR_CHECK(esp_netif_init());
	esp_netif_config_t netif_cfg = ESP_NETIF_DEFAULT_ETH(); // apply default network interface configuration for Ethernet
	s_esp_netif = esp_netif_new(&netif_cfg); // create network interface for Ethernet driver
	// Set default handlers to process TCP/IP stuffs
	ESP_ERROR_CHECK(esp_eth_set_default_handlers(s_esp_netif));
	// Register user defined IP event handler
	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler, NULL));

	/* Create MAC and PHY Instance */
	eth_mac_config_t eth_mac_config = ETH_MAC_DEFAULT_CONFIG(); // apply default MAC configuration
	eth_mac_config.smi_mdc_gpio_num = ETH_MDC_GPIO;
	eth_mac_config.smi_mdio_gpio_num = ETH_MDIO_GPIO;
	esp_eth_mac_t *s_eth_mac = esp_eth_mac_new_esp32(&eth_mac_config); // create MAC instance

	eth_phy_config_t eth_phy_config = ETH_PHY_DEFAULT_CONFIG(); // apply default PHY configuration
	eth_phy_config.phy_addr = ETH_PHY_ADDR;
	eth_phy_config.reset_gpio_num = ETH_PHY_RST_GPIO;
	#ifdef OLIMEX_ESP32_POE_CONFIG // Olimex ESP32-PoE configuration
		gpio_pad_select_gpio(ETH_PHY_POWER_PIN);
		gpio_set_direction(ETH_PHY_POWER_PIN,GPIO_MODE_OUTPUT);
		gpio_set_level(ETH_PHY_POWER_PIN, 1);
	#endif
	esp_eth_phy_t *s_eth_phy = esp_eth_phy_new_lan8720(&eth_phy_config); // create PHY instance

	/* Install Ethernet driver */
	esp_eth_config_t eth_config = ETH_DEFAULT_CONFIG(s_eth_mac, s_eth_phy); // apply default driver configuration
	ESP_ERROR_CHECK(esp_eth_driver_install(&eth_config, &s_eth_handle)); // install driver

	/* Attach Ethernet driver to TCP/IP stack */
	s_eth_glue = esp_eth_new_netif_glue(s_eth_handle);
	ESP_ERROR_CHECK(esp_netif_attach(s_esp_netif, s_eth_glue));

	/* Start Ethernet driver state machine */
	ESP_ERROR_CHECK(esp_eth_start(s_eth_handle));

	return s_esp_netif;
}

esp_err_t eth_connect(void)
{
	if (s_semph_get_ip_addr != NULL) {
		return ESP_ERR_INVALID_STATE;
	}
	s_esp_netif = eth_start();
	ESP_ERROR_CHECK(esp_register_shutdown_handler(&eth_stop));
	ESP_LOGI(TAG, "Waiting for IP address");
	xSemaphoreTake(s_semph_get_ip_addr, portMAX_DELAY);

	return ESP_OK;
}

void eth_stop(void)
{
	ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler));
	ESP_ERROR_CHECK(esp_eth_stop(s_eth_handle));
	ESP_ERROR_CHECK(esp_eth_del_netif_glue(s_eth_glue));
	ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler));
	ESP_ERROR_CHECK(esp_eth_driver_uninstall(s_eth_handle));
	ESP_ERROR_CHECK(s_eth_phy->del(s_eth_phy));
	ESP_ERROR_CHECK(s_eth_mac->del(s_eth_mac));

	esp_netif_destroy(s_esp_netif);
	s_esp_netif = NULL;
}

esp_err_t eth_disconnect(void)
{
    if (s_semph_get_ip_addr == NULL) {
        return ESP_ERR_INVALID_STATE;
    }
    vSemaphoreDelete(s_semph_get_ip_addr);
    s_semph_get_ip_addr = NULL;
    eth_stop();
    ESP_ERROR_CHECK(esp_unregister_shutdown_handler(&eth_stop));
    return ESP_OK;
}