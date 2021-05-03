/*
 * usb_conf.h
 */

#ifndef INC_USB_CONF_H_
#define INC_USB_CONF_H_

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* External variables --------------------------------------------------------*/

/*-------------------------------------------------------------*/
/* EP_NUM */
/* defines how many endpoints are used by the device */
/*-------------------------------------------------------------*/

#define EP_NUM              (4)

//#define CTR_CALLBACK
//#define DOVR_CALLBACK
//#define ERR_CALLBACK
//#define WKUP_CALLBACK
//#define SUSP_CALLBACK
//#define RESET_CALLBACK
//#define SOF_CALLBACK
//#define ESOF_CALLBACK

/* CTR service routines
 * associated to defined endpoints */
//#define  EP1_IN_Callback    NOP_Process
#define  EP2_IN_Callback    NOP_Process
#define  EP3_IN_Callback    NOP_Process
#define  EP4_IN_Callback    NOP_Process
#define  EP5_IN_Callback    NOP_Process
#define  EP6_IN_Callback    NOP_Process
#define  EP7_IN_Callback    NOP_Process

#define  EP1_OUT_Callback   NOP_Process
#define  EP2_OUT_Callback   NOP_Process
//#define  EP3_OUT_Callback   NOP_Process
#define  EP4_OUT_Callback   NOP_Process
#define  EP5_OUT_Callback   NOP_Process
#define  EP6_OUT_Callback   NOP_Process
#define  EP7_OUT_Callback   NOP_Process

#endif /* INC_USB_CONF_H_ */
