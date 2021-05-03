/*
 * usb_prop.h
 */

#ifndef USB_PROP_H_
#define USB_PROP_H_

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct {
  uint32_t bitrate;
  uint8_t format;
  uint8_t paritytype;
  uint8_t datatype;
} LINE_CODING;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define LOBYTE(x)   ((u8)((x >> 0) & 0xFF))
#define HIBYTE(x)   ((u8)((x >> 8) & 0xFF))

/* Exported define -----------------------------------------------------------*/
#define USB_DEVICE_VID                          0x0483
#define USB_DEVICE_PID                          0x7540
#define USB_DEVICE_LANG_ID                      0x0409 /* U.S. English */
#define USB_DEVICE_VERSION_MAJOR                1
#define USB_DEVICE_VERSION_MINOR                0

#define USB_DEVICE_VENDOR_STRING                "MindMotion"
#define USB_DEVICE_PRODUCT_STRING               "MindMotion Virtual COM Port"
#define USB_DEVICE_SERIAL_NUMBER_PREFIX         "BASTION_USB2CAN:"

//#define USB_VirtualComPort_GetConfiguration     NOP_Process
//#define USB_VirtualComPort_SetConfiguration     NOP_Process
#define USB_VirtualComPort_GetInterface         NOP_Process
#define USB_VirtualComPort_SetInterface         NOP_Process
#define USB_VirtualComPort_GetStatus            NOP_Process
#define USB_VirtualComPort_ClearFeature         NOP_Process
#define USB_VirtualComPort_SetEndPointFeature   NOP_Process
#define USB_VirtualComPort_SetDeviceFeature     NOP_Process
//#define Virtual_Com_Port_SetDeviceAddress       NOP_Process

#define SEND_ENCAPSULATED_COMMAND           0x00
#define GET_ENCAPSULATED_RESPONSE           0x01
#define SET_COMM_FEATURE                    0x02
#define GET_COMM_FEATURE                    0x03
#define CLEAR_COMM_FEATURE                  0x04
#define SET_LINE_CODING                     0x20
#define GET_LINE_CODING                     0x21
#define SET_CONTROL_LINE_STATE              0x22
#define SEND_BREAK                          0x23

/* Exported functions ------------------------------------------------------- */
#ifdef __cplusplus
extern "C" {
#endif

//@formatter:off

void     USB_VirtualComPort_Init();
void     USB_VirtualComPort_Reset();
void     USB_VirtualComPort_GetConfiguration();
void     USB_VirtualComPort_SetConfiguration();
void     USB_VirtualComPort_SetDeviceAddress();
void     USB_VirtualComPort_StatusIn();
void     USB_VirtualComPort_StatusOut();
RESULT   USB_VirtualComPort_DataSetup(uint8_t RequestNo);
RESULT   USB_VirtualComPort_NoDataSetup(uint8_t);
RESULT   USB_VirtualComPort_GetInterfaceSetting(uint8_t Interface, uint8_t AlternateSetting);
uint8_t* USB_VirtualComPort_GetDeviceDescriptor(uint16_t Length);
uint8_t* USB_VirtualComPort_GetConfigDescriptor(uint16_t Length);
uint8_t* USB_VirtualComPort_GetStringDescriptor(uint16_t Length);

uint8_t* USB_VirtualComPort_GetLineCoding(uint16_t Length);
uint8_t* USB_VirtualComPort_SetLineCoding(uint16_t Length);

//@formatter:on

#ifdef __cplusplus
}
#endif

#endif /* USB_PROP_H_ */
