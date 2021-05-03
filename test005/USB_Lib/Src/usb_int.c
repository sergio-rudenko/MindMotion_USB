////////////////////////////////////////////////////////////////////////////////
/// @file     usb_int.C
/// @author   AE TEAM
/// @brief    Endpoint CTR (Low and High) interrupt's service routines.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion
#define _USB_INT_C_
// Files includes

#include "usb_lib.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_INIT
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{

extern void (*pEpInt_IN[7])(void);    //  Handles IN  interrupts
extern void (*pEpInt_OUT[7])(void);   //  Handles OUT interrupts

////////////////////////////////////////////////////////////////////////////////
/// @brief  Low priority Endpoint Correct Transfer interrupt's service
///         routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void CTR_LP(void)
{
  u32 wEpIntSta = 0, wEpxIntSta = 0;
  u8 i;

  /*  Clear endpoint interrupt */
  _ClrUSB_INT_STA(USB_INT_STATE_EPINTF);

  /* Gets the interrupt endpoint number */
  wEpIntSta = _GetEP_INT_STA();

  /* endpoint 0 */
  if (wEpIntSta & EP_INT_STATE_EP0F)
  {
    EPindex = 0;
    _ClrEP_INT_STA(EP_INT_STATE_EP0F);

    /* Gets the endpoint 0 interrupt status */
    wEpxIntSta = _GetEP0_INT_STA();

    if (wEpxIntSta & EPn_INT_STATE_SETUP)
    {
      _ClrEP0_INT_STA(EPn_INT_STATE_SETUP);
      Setup0_Process();
      return;
    }

    if (wEpxIntSta & (EPn_INT_STATE_OUTACK /* | EPn_INT_STATE_OUTNACK */))
    {
      _ClrEP0_INT_STA(EPn_INT_STATE_OUTACK | EPn_INT_STATE_OUTNACK);
      Out0_Process();
      return;
    }

    if (wEpxIntSta & (EPn_INT_STATE_INACK /* | EPn_INT_STATE_INNACK */))
    {
      _ClrEP0_INT_STA(EPn_INT_STATE_INACK | EPn_INT_STATE_INNACK);
      In0_Process();
      return;
    }
  } /* endpoint 0 */

  /* endpoint N */
  if (wEpIntSta & (~EP_INT_STATE_EP0F))
  {
    for (i = 1; i < 5; i++)
    {
      if (wEpIntSta & (1 << i))
      {
        EPindex = i;
        _ClrEP_INT_STA(0x1 << EPindex);

        /* Gets the endpoint interrupt status */
        wEpxIntSta = _GetEPn_INT_STA(EPindex);

        if (wEpxIntSta & (EPn_INT_STATE_INACK | EPn_INT_STATE_INNACK))
        {
          _ClrEPn_INT_STA(EPn_INT_STATE_INACK | EPn_INT_STATE_INNACK, EPindex);

          /* call IN service function */
          (*pEpInt_IN[EPindex - 1])();    \
        }

        if (wEpxIntSta & EPn_INT_STATE_OUTACK)
        {
          _ClrEPn_INT_STA(EPn_INT_STATE_OUTACK | EPn_INT_STATE_OUTNACK, EPindex);

          /* call OUT service function */
          (*pEpInt_OUT[EPindex - 1])();
        }
      }
    }
  } /* endpoint N */
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  High Priority Endpoint Correct Transfer interrupt's service
///         routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void CTR_HP(void)
{

}

/// @}

/// @}

/// @}

