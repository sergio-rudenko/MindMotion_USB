/*
 * hw_config.c
 */

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "usb_lib.h"


/*******************************************************************************
 * Function Name  : USB_HardwareInit
 * Description    : Low level (Peripheral) initialisation
 * Input          : None
 * Return         : None
 *******************************************************************************/
void USB_HardwareInit()
{
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Set PLL clock as USB 48MHz clock */
  RCC->CFGR &= ~(1 << 15); // CLK48SEL set to PLL
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_Div1);

  /* Enable USB Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);

  /* USB_DP and USB_DM pins configure */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  // Analog input
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* USB Interrupt configure */
  NVIC_InitStructure.NVIC_IRQChannel = USB_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
 * Function Name  : USB_CableConfig
 * Description    : Software Connection/Disconnection of USB Cable
 * Input          : None
 * Return         : None
 *******************************************************************************/
void USB_CableConfig(FunctionalState NewState)
{
  if (NewState == ENABLE)
  {
    _ClrUSB_TOP(USB_TOP_CONNECT);
  }
  else
  {
    _SetUSB_TOP(USB_TOP_CONNECT);
  }
}
