/*
 * usb_pwr.h
 */

#ifndef USB_PWR_H_
#define USB_PWR_H_

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum _RESUME_STATE
{
  RESUME_EXTERNAL,
  RESUME_INTERNAL,
  RESUME_LATER,
  RESUME_WAIT,
  RESUME_START,
  RESUME_ON,
  RESUME_OFF,
  RESUME_ESOF
} RESUME_STATE;

typedef enum _DEVICE_STATE
{
  UNCONNECTED,
  ATTACHED,
  POWERED,
  SUSPENDED,
  ADDRESSED,
  CONFIGURED
} DEVICE_STATE;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//@formatter:off
#ifdef __cplusplus
extern "C" {
#endif

void   Suspend();
void   Resume_Init();
void   Resume(RESUME_STATE eResumeSetVal);
RESULT USB_DevicePowerOn();
RESULT USB_DevicePowerOff();

#ifdef __cplusplus
}
#endif

/* External variables --------------------------------------------------------*/
extern __IO uint32_t bDeviceState;      // USB device status
extern __IO bool     fSuspendEnabled;   // true when suspend is possible

//@formatter:on
#endif /* USB_PWR_H_ */
