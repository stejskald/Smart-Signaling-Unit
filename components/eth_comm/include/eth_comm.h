/**
 * @file eth_comm.h
 * @author David Stejskal (xstejs30@stud.feec.vutbr.cz)
 * @brief Implementation of common functions for Ethernet communication.
 * @version 0.1
 * @date 2021-04-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __ETH_COMM_H__
#define __ETH_COMM_H__
#include "sdkconfig.h"	// for KConfig options
#include "esp_event.h"	// for event handling

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Event handler for Ethernet events
 * 
 * @param arg 
 * @param event_base 
 * @param event_id 
 * @param event_data 
 */
void eth_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

/**
 * @brief Event handler for IP_EVENT_ETH_GOT_IP
 * 
 * @param arg 
 * @param event_base 
 * @param event_id 
 * @param event_data 
 */
void got_ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

/**
 * @brief Set up Ethernet connection
 * 
 * @return esp_netif_t* of created network interface for Ethernet
 */
esp_netif_t *eth_start(void);

/**
 * @brief Configure Ethernet, connect, wait for IP
 * 
 * @return ESP_OK on successful connection
 */
esp_err_t eth_connect(void);

/**
 * @brief Tear down connection, release resources
 * 
 */
void eth_stop(void);

/**
 * @brief Counterpart to eth_connect, de-initialize Ethernet
 * 
 * @return ESP_OK on successful disconnection
 */
esp_err_t eth_disconnect(void);

/**
 * @brief Get the netif object
 * 
 * @return esp_netif_t* 
 */
esp_netif_t *get_netif(void);

#ifdef __cplusplus
}
#endif

#endif /* __ETH_COMM_H__ */
