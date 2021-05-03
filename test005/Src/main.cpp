/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------
#include "main.h"
#include "Timer.h"

#include "hw_config.h"
#include "usb_lib.h"
//#include "usb_desc.h"
//#include "usb_pwr.h"

//extern __IO u32 gTxDataCnt;
//extern u8 gTxData[VIRTUAL_COM_PORT_DATA_SIZE];
//extern u8 gUartBuffCnt;
//
//uint8_t tx_buffer[128];
//uint8_t rx_buffer[128];

// ----------------------------------------------------------------------------
//
// Print a greeting message on the trace device and enter a loop
// to count seconds.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the NONE output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//
// ----------------------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(int argc, char *argv[])
{
  USB_HardwareInit();
  USB_Init();

  // Normally at this stage most of the microcontroller subsystems, including
  // the clock, were initialised by the CMSIS SystemInit() function invoked
  // from the startup file, before calling main().
  // (see system/src/cortexm/_initialize_hardware.c)
  // If further initialisations are required, customise __initialize_hardware()
  // or add the additional initialisation here, for example:
  //
  // HAL_Init();

  // In this sample the SystemInit() function is just a placeholder,
  // if you do not add the real one, the clock will remain configured with
  // the reset value, usually a relatively low speed RC clock (8-12MHz).

  // Send a greeting to the trace device (skipped on Release).
  trace_puts("Hello Arm World!");

  // At this stage the system clock should have already been configured
  // at high speed.
  trace_printf("System clock: %u Hz\n", SystemCoreClock);

  Timer timer;
  timer.start();

  int seconds = 0;

  // Infinite loop
  while (1)
  {
    timer.sleep(Timer::FREQUENCY_HZ);

    ++seconds;

    // Count seconds on the trace device.
    trace_printf("Second %d\n", seconds);

//    if ((gTxDataCnt != 0) && (bDeviceState == ADDRESSED))
//    {
//      int index = 0;
//
////			USB_To_UART_Send_Data(gTxData, &gUartBuffCnt);
//
//      while (gUartBuffCnt)
//      {
//        rx_buffer[index] = gTxData[index];
//        gUartBuffCnt--;
//        index++;
//      }
//    }
  }
  // Infinite loop, never return.
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles USB Low Priority interrupts
///         requests
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
extern "C" void USB_IRQHandler(void)
{
//  USB_Istr();
}

