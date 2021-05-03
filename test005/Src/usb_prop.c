/*
 * usb_prop.c
 */

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_pwr.h"
#include "usb_conf.h"
#include "usb_desc.h"
#include "usb_prop.h"

#include <string.h>

/* Private variables ---------------------------------------------------------*/
uint8_t Request = 0;

LINE_CODING
linecoding =
    {
     9600,  // baud rate
     0x00,  // stop bits-1
     0x00,  // parity - none
     0x08   // no. of bits 8
    };

/* Extern variables ----------------------------------------------------------*/

/* -------------------------------------------------------------------------- */
/*  Structures initialisations */
/* -------------------------------------------------------------------------- */

DEVICE
Device_Table = {EP_NUM, 1};

DEVICE_PROP
Device_Property =
    {
     USB_VirtualComPort_Init,
     USB_VirtualComPort_Reset,
     USB_VirtualComPort_StatusIn,
     USB_VirtualComPort_StatusOut,
     USB_VirtualComPort_DataSetup,
     USB_VirtualComPort_NoDataSetup,
     USB_VirtualComPort_GetInterfaceSetting,
     USB_VirtualComPort_GetDeviceDescriptor,
     USB_VirtualComPort_GetConfigDescriptor,
     USB_VirtualComPort_GetStringDescriptor,
     0,
     0x40 /* MAX PACKET SIZE */
    };

USER_STANDARD_REQUESTS
User_Standard_Requests =
    {
     USB_VirtualComPort_GetConfiguration,
     USB_VirtualComPort_SetConfiguration,
     USB_VirtualComPort_GetInterface,
     USB_VirtualComPort_SetInterface,
     USB_VirtualComPort_GetStatus,
     USB_VirtualComPort_ClearFeature,
     USB_VirtualComPort_SetEndPointFeature,
     USB_VirtualComPort_SetDeviceFeature,
     USB_VirtualComPort_SetDeviceAddress
    };

static const ONE_DESCRIPTOR
USB_Device_Descriptor =
    {
     (u8*)USB_VirtualComPort_DeviceDescriptor,
     VIRTUAL_COM_PORT_SIZ_DEVICE_DESC
    };

static const ONE_DESCRIPTOR
USB_Config_Descriptor =
    {
     (u8*)USB_VirtualComPort_ConfigDescriptor,
     VIRTUAL_COM_PORT_SIZ_CONFIG_DESC
    };

static ONE_DESCRIPTOR
String_Descriptor =
    {
     (u8*)USB_VirtualComPort_StringDescriptor,
     0
    };

/*******************************************************************************
 * Function Name  : USB_VirtualComPort_Init
 * Description    : Init routine
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USB_VirtualComPort_Init()
{
  pInformation->Current_Configuration = 0;

  /* Connect the device */
  USB_DevicePowerOn();

  /* USB interrupts initialisation */
  _ClrUSB_INT_STA(0xff);

  //TODO: Rx and Tx buffer initialisation

  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
 * Function Name  : USB_VirtualComPort_Reset
 * Description    : Reset routine
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USB_VirtualComPort_Reset()
{
  /* Set Virtual_Com_Port DEVICE as not configured */
  pInformation->Current_Configuration = 0;

  /* Current Feature initialization */
  pInformation->Current_Feature = USB_VirtualComPort_ConfigDescriptor[7];

  /* Set Virtual_Com_Port DEVICE with the default Interface */
  pInformation->Current_Interface = 0;

  /* Disable endpoints */
  _ClrEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3));

  /* Setup EP0 */
  _SetEP0_INT_EN(EPn_INT_EN_SETUPIE | EPn_INT_EN_INNACKIE | EPn_INT_EN_OUTACKIE); //EPn_INT_EN_INACKIE|EPn_INT_EN_OUTACKIE
  _ClrEP0_INT_STA(EPn_INT_EN_INACKIE | EPn_INT_STATE_SETUP | EPn_INT_STATE_INNACK | EPn_INT_STATE_OUTACK);

  /* Setup EP1 */
  _SetEP1_INT_EN(EPn_INT_EN_INNACKIE); //| EPn_INT_EN_INACKIE
  _ClrEP1_INT_STA(EPn_INT_STATE_INNACK); //| EPn_INT_STATE_INACK

  /* Setup EP2 */
  _SetEP2_INT_EN(EPn_INT_EN_INNACKIE); //| EPn_INT_EN_INACKIE
  _ClrEP2_INT_STA(EPn_INT_EN_INNACKIE); //| EPn_INT_EN_INACKIE

  /* Setup EP3 */
  _SetEP3_INT_EN(EPn_INT_EN_OUTACKIE);
  _ClrEP3_INT_STA(EPn_INT_STATE_OUTACK);

  /* Enable endpoints */
  _SetEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3));
  _SetEP_INT_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3));

  SetDeviceAddress(0);

  _SetUSB_TOP(USB_TOP_RESET);
  _ClrUSB_TOP(USB_TOP_RESET);

  bDeviceState = ATTACHED;
}

/*******************************************************************************
 * Function Name  : USB_VirtualComPort_GetConfiguration
 * Description    :
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USB_VirtualComPort_GetConfiguration()
{
}

/*******************************************************************************
 * Function Name  : USB_VirtualComPort_SetConfiguration
 * Description    : Update the device state to configured
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USB_VirtualComPort_SetConfiguration()
{
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}

/*******************************************************************************
 * Function Name  : USB_VirtualComPort_SetDeviceAddress.
 * Description    : Update the device state to addressed.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void USB_VirtualComPort_SetDeviceAddress()
{
  bDeviceState = ADDRESSED;
}

/*******************************************************************************
 * Function Name  : USB_VirtualComPort_StatusIn
 * Description    : Virtual COM Port Status In Routine.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USB_VirtualComPort_StatusIn()
{
  bDeviceState = ADDRESSED;
}

/*******************************************************************************
 * Function Name  : USB_VirtualComPort_StatusOut
 * Description    : Virtual COM Port Status OUT Routine
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USB_VirtualComPort_StatusOut()
{
}

/*******************************************************************************
 * Function Name  : USB_VirtualComPort_DataSetup
 * Description    : handle the data class specific requests
 * Input          : Request Nb
 * Output         : None
 * Return         : USB_UNSUPPORT or USB_SUCCESS
 *******************************************************************************/
RESULT USB_VirtualComPort_DataSetup(u8 RequestNo)
{
  u8* (*CopyRoutine)(u16);
  CopyRoutine = NULL;

  if (RequestNo == GET_LINE_CODING)
  {
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
    {
      CopyRoutine = USB_VirtualComPort_GetLineCoding;
    }
  }
  else
  if (RequestNo == SET_LINE_CODING)
  {
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
    {
      CopyRoutine = USB_VirtualComPort_SetLineCoding;
    }
    Request = SET_LINE_CODING;
  }

  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }

  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;

  (*CopyRoutine)(0);

  return USB_SUCCESS;
}

/*******************************************************************************
 * Function Name  : USB_VirtualComPort_NoDataSetup
 * Description    : handle the no data class specific requests.
 * Input          : Request Nb
 * Output         : None
 * Return         : USB_UNSUPPORT or USB_SUCCESS
 *******************************************************************************/
RESULT USB_VirtualComPort_NoDataSetup(u8 RequestNo)
{
  if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
  {
    if (RequestNo == SET_COMM_FEATURE)
    {
      return USB_SUCCESS;
    }
    else
    if (RequestNo == SET_CONTROL_LINE_STATE)
    {
      return USB_SUCCESS;
    }
  }
  return USB_UNSUPPORT;
}

/*******************************************************************************
 * Function Name  : USB_VirtualComPort_GetDeviceDescriptor
 * Description    : Gets the device descriptor
 * Input          : Length
 * Output         : None
 * Return         : The address of the device descriptor
 *******************************************************************************/
u8* USB_VirtualComPort_GetDeviceDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, (ONE_DESCRIPTOR*)&USB_Device_Descriptor);
}

/*******************************************************************************
 * Function Name  : USB_VirtualComPort_GetConfigDescriptor
 * Description    : get the configuration descriptor
 * Input          : Length
 * Output         : None
 * Return         : The address of the configuration descriptor
 *******************************************************************************/
u8* USB_VirtualComPort_GetConfigDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, (ONE_DESCRIPTOR*)&USB_Config_Descriptor);
}

/*******************************************************************************
 * Function Name  : USB_VirtualComPort_GetStringDescriptor
 * Description    : Gets the string descriptors according to the needed index
 * Input          : Length
 * Output         : None
 * Return         : The address of the string descriptors
 *******************************************************************************/
u8* USB_VirtualComPort_GetStringDescriptor(u16 Length)
{
  u8 wValue0 = pInformation->USBwValue0;
  const char *p;

  if (wValue0 == 0) /* Language ID */
  {
    USB_VirtualComPort_StringDescriptor[1] = USB_STRING_DESCRIPTOR_TYPE;
    USB_VirtualComPort_StringDescriptor[2] = LOBYTE(USB_DEVICE_LANG_ID);
    USB_VirtualComPort_StringDescriptor[3] = HIBYTE(USB_DEVICE_LANG_ID);

    USB_VirtualComPort_StringDescriptor[0] = VIRTUAL_COM_PORT_SIZ_STRING_LANGID;
    String_Descriptor.Descriptor_Size = USB_VirtualComPort_StringDescriptor[0];
  }
  else
  if (wValue0 == 1) /* Vendor */
  {
    USB_VirtualComPort_StringDescriptor[1] = USB_STRING_DESCRIPTOR_TYPE;

    p = USB_DEVICE_VENDOR_STRING;
    for (size_t i = 0; i < strlen(p); i++)
    {
      USB_VirtualComPort_StringDescriptor[2 + i * 2] = p[i];
      USB_VirtualComPort_StringDescriptor[3 + i * 2] = 0x00;
    }

    USB_VirtualComPort_StringDescriptor[0] = 2 + strlen(p) * 2;
    String_Descriptor.Descriptor_Size = USB_VirtualComPort_StringDescriptor[0];
  }
  if (wValue0 == 2) /* Product */
  {
    USB_VirtualComPort_StringDescriptor[1] = USB_STRING_DESCRIPTOR_TYPE;

    p = USB_DEVICE_PRODUCT_STRING;
    for (size_t i = 0; i < strlen(p); i++)
    {
      USB_VirtualComPort_StringDescriptor[2 + i * 2] = p[i];
      USB_VirtualComPort_StringDescriptor[3 + i * 2] = 0x00;
    }

    USB_VirtualComPort_StringDescriptor[0] = 2 + strlen(p) * 2;
    String_Descriptor.Descriptor_Size = USB_VirtualComPort_StringDescriptor[0];
  }
  if (wValue0 == 3) /* Serial Number */
  {
    USB_VirtualComPort_StringDescriptor[1] = USB_STRING_DESCRIPTOR_TYPE;

    p = USB_DEVICE_SERIAL_NUMBER_PREFIX;
    for (size_t i = 0; i < strlen(p); i++)
    {
      USB_VirtualComPort_StringDescriptor[2 + i * 2] = p[i];
      USB_VirtualComPort_StringDescriptor[3 + i * 2] = 0x00;
    }

    //TODO: Serial

    USB_VirtualComPort_StringDescriptor[0] = 2 + strlen(p) * 2;
    String_Descriptor.Descriptor_Size = USB_VirtualComPort_StringDescriptor[0];
  }
  else
  {
    return NULL;
  }

  return Standard_GetDescriptorData(Length, &String_Descriptor);
}

/*******************************************************************************
 * Function Name  : USB_VirtualComPort_GetInterfaceSetting
 * Description    : test the interface and the alternate setting according to the
 *                  supported one
 * Input1         : uint8_t: Interface : interface number
 * Input2         : uint8_t: AlternateSetting : Alternate Setting number
 * Output         : None
 * Return         : The address of the string descriptors
 *******************************************************************************/
RESULT USB_VirtualComPort_GetInterfaceSetting(u8 Interface, u8 AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > 1)
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/*******************************************************************************
 * Function Name  : USB_VirtualComPort_GetLineCoding
 * Description    : send the linecoding structure to the PC host
 * Input          : Length
 * Output         : None
 * Return         : Linecoding structure base address
 *******************************************************************************/
u8* USB_VirtualComPort_GetLineCoding(u16 Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = sizeof(linecoding);
    return NULL;
  }
  return (u8*)&linecoding;
}

/*******************************************************************************
 * Function Name  : USB_VirtualComPort_SetLineCoding
 * Description    : Set the linecoding structure fields
 * Input          : Length
 * Output         : None
 * Return         : Linecoding structure base address
 *******************************************************************************/
u8* USB_VirtualComPort_SetLineCoding(u16 Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = sizeof(linecoding);
    return NULL;
  }
  return (u8*)&linecoding;
}
