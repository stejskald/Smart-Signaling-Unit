#ifndef __MB_TCP_SERVER_H__
#define __MB_TCP_SERVER_H__
//#include "esp_err.h"	// for error handling

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialization of NVS Flash
 * 
 */
void nvs_flash_initialization(void);

void mb_server_test(void);

#ifdef __cplusplus
}
#endif

#endif /* __MB_TCP_SERVER_H__ */