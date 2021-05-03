/*
 * hw_config.h
 */

#ifndef INC_HW_CONFIG_H_
#define INC_HW_CONFIG_H_

/* Includes ------------------------------------------------------------------*/
#include <stddef.h> /* for size_t */
#include "hal_conf.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef __cplusplus
extern "C" {
#endif

void USB_HardwareInit(void);
void USB_CableConfig(FunctionalState NewState);
void Get_SerialNum(void);

size_t CDC_Send_DATA (uint8_t *ptrBuffer, uint8_t Send_length);
size_t CDC_Receive_DATA(void);

#ifdef __cplusplus
}
#endif

/* External variables --------------------------------------------------------*/

#endif /* INC_HW_CONFIG_H_ */
