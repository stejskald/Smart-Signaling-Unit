#ifndef __ETH_COMM_H__
#define __ETH_COMM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sdkconfig.h"
#include "esp_eth.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "esp_event.h"
#include "lwip/err.h" // Needed?
#include "lwip/sys.h" // Needed?

#define OLIMEX_ESP32_POE_CONFIG
#ifdef OLIMEX_ESP32_POE_CONFIG
/**
 * @brief Ethernet Connection defines - Olimex ESP32-PoE configuration
 * 
 */
#define ETH_MDC_GPIO		(23) // Output to PHY
#define ETH_MDIO_GPIO		(18) // Bidirectional
#define ETH_PHY_RST_GPIO	(-1)
#define ETH_PHY_ADDR		(0)
#define ETH_PHY_POWER_PIN	(12)

#else // TODO: Check up the GPIO numbers for SSU
#define ETH_MDC_GPIO		(23) // Output to PHY
#define ETH_MDIO_GPIO		(18) // Bidirectional
#define ETH_PHY_RST_GPIO	(-1)
#define ETH_PHY_ADDR		(0)
#define ETH_PHY_POWER_PIN	(12)
#endif

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
 * @brief Set up Ethernet connection
 * 
 * @return esp_netif_t* 
 */
static esp_netif_t *eth_start(void);

/**
 * @brief Configure Ethernet, connect, wait for IP
 * 
 * @return ESP_OK on successful connection
 */
esp_err_t eth_connect(void); // TODO: return esp_err_t

/**
 * @brief Tear down connection, release resources
 * 
 */
static void eth_stop(void);

/**
 * @brief Counterpart to eth_connect, de-initialize Ethernet
 * 
 * @return ESP_OK on successful disconnection
 */
esp_err_t eth_disconnect(void);

#ifdef __cplusplus
}
#endif

#endif /* __ETH_COMM_H__ */
