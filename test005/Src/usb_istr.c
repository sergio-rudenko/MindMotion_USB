/*
 * usb_istr.c
 */

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
//#include "usb_prop.h"
//#include "usb_pwr.h"
#include "usb_istr.h"

/* Variables -----------------------------------------------------------------*/
__IO u16 wIstr;  // ISTR register last read value
__IO u8 bIntPackSOF = 0;  // SOFs received between 2 consecutive packets

void (*pEpInt_IN[7])(void) =
{
  EP1_IN_Callback,
  EP2_IN_Callback,
  EP3_IN_Callback,
  EP4_IN_Callback,
  EP5_IN_Callback,
  EP6_IN_Callback,
  EP7_IN_Callback,
};

void (*pEpInt_OUT[7])(void) =
{
  EP1_OUT_Callback,
  EP2_OUT_Callback,
  EP3_OUT_Callback,
  EP4_OUT_Callback,
  EP5_OUT_Callback,
  EP6_OUT_Callback,
  EP7_OUT_Callback,
};

////////////////////////////////////////////////////////////////////////////////
/// @brief  STR events interrupt service routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void USB_Istr(void)
{
  wIstr = _GetUSB_INT_STA();

  if (wIstr & USB_INT_STATE_RSTF)
  {
    _ClrUSB_INT_STA(USB_INT_STATE_RSTF);
    Device_Property.Reset();
  }

  if (wIstr & USB_INT_STATE_SOFF)
  {
    bIntPackSOF++;
  }

  if (wIstr & USB_INT_STATE_EPINTF)
  {
    CTR_LP();
  }
}

