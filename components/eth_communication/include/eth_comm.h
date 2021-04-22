#ifndef __ETH_COMM_H__
#define __ETH_COMM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_log.h"
#include "esp_event.h"

/**
 * @brief Ethernet Connection defines - Olimex ESP32-PoE configuration
 * 
 */
#define ETH_MDC_GPIO		(23) // Output to PHY
#define ETH_MDIO_GPIO		(18) // Bidirectional
#define ETH_PHY_RST_GPIO	(-1)
#define ETH_PHY_ADDR		(0)
#define ETH_PHY_POWER_PIN	(12)

#define ETH_TAG "ETH"

/**
 * @brief Event handler for Ethernet events
 * 
 * @param arg 
 * @param event_base 
 * @param event_id 
 * @param event_data 
 */
static void eth_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

/**
 * @brief Event handler for IP_EVENT_ETH_GOT_IP
 * 
 * @param arg 
 * @param event_base 
 * @param event_id 
 * @param event_data 
 */
static void got_ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

/**
 * @brief Configure Ethernet, connect, wait for IP
 * 
 * @return ESP_OK on successful connection
 */
void eth_connect(void); // TODO: return esp_err_t

/**
 * @brief Counterpart to eth_connect, de-initialize Ethernet
 * 
 */
void eth_disconnect(void);

/**
 * @brief Returns esp-netif pointer created by eth_connect()
 *
 * @note If multiple interfaces active at once, this API returns NULL
 */
esp_netif_t *get_example_netif(void);

#ifdef __cplusplus
}
#endif

#endif /* __ETH_COMM_H__ */
