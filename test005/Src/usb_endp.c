/*
 * usb_endp.c
 */
/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
//#include "usb_desc.h"
//#include "usb_mem.h"
//#include "hw_config.h"
//#include "usb_istr.h"
//#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Interval between sending IN packets in frame number (1 frame = 1ms) */
#define VCOMPORT_IN_FRAME_INTERVAL             5
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : EP1_IN_Callback
 * Description    :
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
extern __IO u8 bIntPackSOF ;
void EP1_IN_Callback(void)
{
    if( bIntPackSOF > 1) { // Receive data length 64byte
//        if((_GetUSB_CTRL1() & EP1_CTRL_TRANEN ) == 0) {
//            UserToPMABufferCopy(buffer_in, ENDP1, count_in);
//            if(count_in > 0) {
//                _SetUSB_CTRL1(EP1_CTRL_TRANEN | count_in) ;
//            }
//            count_in = 0;
//            status_UsbUart &= ~(1 << 5);
//        }
    }
}

/*******************************************************************************
 * Function Name  : EP3_OUT_Callback
 * Description    :
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void EP3_OUT_Callback(void)
{
//  PMAToUserBufferCopy(gTxData, ENDP3, _GetUSB_AVILn(ENDP3));
}
