/*
 * usb_pwr.c
 */

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t bDeviceState = UNCONNECTED; /* USB device status */

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * Function Name  : PowerOn
  * Description    :
  * Input          : None
  * Output         : None
  * Return         : USB_SUCCESS
  */
RESULT
USB_DevicePowerOn(void)
{

    USB_CableConfig(DISABLE);          //USB connection 1.5K internal pull-up resistor

    _SetUSB_TOP(USB_TOP_RESET) ;        //USB reset
    _ClrUSB_TOP(USB_TOP_RESET) ;

    _ClrUSB_INT_STA(USB_INT_STATE_RSTF | USB_INT_STATE_EPINTF); //Clear USB interruptUSB_INT_STATE_SOFF|

    _SetUSB_INT_EN(USB_INT_EN_RSTIE | USB_INT_EN_EPINTIE);      //Enabler USB interruptUSB_INT_EN_SOFIE|

    _SetEP0_INT_EN( EPn_INT_EN_SETUPIE | EPn_INT_EN_INNACKIE | EPn_INT_EN_OUTACKIE); //|EPn_INT_EN_OUTNACKIE

    _ClrEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3) | (1 << ENDP4));

    _SetEP0_INT_EN( EPn_INT_EN_SETUPIE | EPn_INT_EN_INNACKIE | \
                    EPn_INT_EN_OUTACKIE);//|EPn_INT_EN_OUTNACKIE|//EPn_INT_EN_INACKIE|
    _ClrEP0_INT_STA(EPn_INT_STATE_SETUP | EPn_INT_STATE_INNACK | \
                    EPn_INT_EN_INACKIE | EPn_INT_STATE_OUTACK | EPn_INT_STATE_OUTNACK);


    _SetEP1_INT_EN( EPn_INT_EN_INNACKIE );                      // EPn_INT_EN_INACKIE
    _ClrEP1_INT_STA(EPn_INT_STATE_INNACK);                      // EPn_INT_STATE_INACK


    _SetEP2_INT_EN(EPn_INT_EN_INNACKIE );                       // EPn_INT_EN_INACKIE
    _ClrEP2_INT_STA(EPn_INT_EN_INNACKIE);                       // EPn_INT_EN_INACKIE


    _SetEP3_INT_EN( EPn_INT_EN_OUTACKIE);
    _ClrEP3_INT_STA( EPn_INT_STATE_OUTACK);


    _SetEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3));
    _SetEP_INT_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3));

    USB_CableConfig(ENABLE);                                   //USB connection 1.5K internal pull-up resistor

    return USB_SUCCESS;
}

/**
  * Function Name  : PowerOff
  * Description    : handles switch-off conditions
  * Input          : None
  * Output         : None
  * Return         : USB_SUCCESS
  **/
RESULT USB_DevicePowerOff()
{
    _SetUSB_TOP(USB_TOP_RESET) ;            // USB reset
    _ClrUSB_INT_STA(USB_INT_STATE_RSTF |
                    USB_INT_STATE_SOFF |
                    USB_INT_STATE_EPINTF);  //Clear USB interrupt

    USB_CableConfig(DISABLE);               // Disconnect

    return USB_SUCCESS;
}

