#include "stdio.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "ch554_usb.h"
#include "debug.h"
#include "delay.h"

#define THIS_ENDP0_SIZE         64
#define UsbSetupBuf     		((PUSB_SETUP_REQ)Ep0Buffer)


__xdata __at (0x0000) uint8_t  Ep0Buffer[THIS_ENDP0_SIZE];      			   // Endpoint 0 OUT & IN buffer, must be an even address
__xdata __at (0x0080) uint8_t  Ep2Buffer[2*THIS_ENDP0_SIZE];                   // Endpoint 2 IN & OUT buffer, must be an even address

USB_DigitizerReport_Data_t DigitizerReportData;
USB_DigitizerReport_t  DigitizerReport;

volatile uint16_t SetupLen;
volatile uint16_t remainingLength;
uint8_t SetupReq, Count, UsbConfig, FLAG, Ready;
const uint8_t *  pDescr;                                                       //USB configuration flag
USB_SETUP_REQ   SetupReqBuf;                                                   //Temporary Setup package


__code uint8_t DevDesc[18] = {0x12,0x01,0x01,0x00,0x00,0x00,0x00,THIS_ENDP0_SIZE,
                      0x86,0x1a,0xe3,0xe5,0x00,0x00,0x01,0x02,
                      0x00,0x01
                     };

void CreateReport(USB_DigitizerReport_t* Report)
{
	Report->ReportId = 0x01;
	Report->Data = DigitizerReportData;
}

uint8_t FeatureReport[] = {0x02, 0x05}; // ReportID:2 MaxContactPoints:5


__code uint8_t CfgDesc[34] =
{
	/* Configuration descriptor - 9 bytes */
    0x09,0x02,0x22,0x00,0x01,0x01,0x00,0xa0,0x32,
	/* Interface descriptor - 9 bytes */
    0x09,0x04,0x00,0x00,0x01,0x03,0x00,0x00,0x00,
	/* HID descriptor - 9 bytes */
    0x09,0x21,0x00,0x01,0x00,0x01,0x22,0x89,0x01,				// last 2 bytes (0x89 0x01 is report size which is 393 in DEC)
	/* Endpoint descriptor - 7 bytes */
    0x07,0x05,0x82,0x03,THIS_ENDP0_SIZE,0x00,0x01,

};


__code uint8_t	MyLangDescr[] = { 0x04, 0x03, 0x09, 0x04 };

__code uint8_t	MyManuInfo[] = { 0x0E, 0x03, 'w', 0, 'c', 0, 'h', 0, '.', 0, 'c', 0, 'n', 0 };

__code uint8_t	MyProdInfo[] = { 0x0C, 0x03, 'C', 0, 'H', 0, '5', 0, '5', 0, '4', 0 };


__code const USB_Descriptor_HIDReport_Datatype_t HIDRepDesc[] =
{
	0x05, 0x0D, 			// Usage Page (Digitizer)
	0x09, 0x04, 			// Usage (Touch Screen)
	0xA1, 0x01, 			// Collection (Application)
	0x85, 0x01, 			//     Report ID (1)
	0x09, 0x22, 			//     Usage (Finger)
	0xA1, 0x02, 			//     Collection (Logical)
	0x05, 0x0D, 			//         Usage Page (Digitizer)
	0x09, 0x42, 			//         Usage (Tip Switch)
	0x09, 0x32, 			//         Usage (In Range)
	0x15, 0x00, 			//         Logical Minimum (0)
	0x25, 0x01, 			//         Logical Maximum (1)
	0x95, 0x02, 			//         Report Count (2)
	0x75, 0x01, 			//         Report Size (1)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x95, 0x06, 			//         Report Count (6)
	0x81, 0x03, 			//         Input (Cnst,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x09, 0x51, 			//         Usage (Contact Identifier)
	0x95, 0x01, 			//         Report Count (1)
	0x75, 0x08, 			//         Report Size (8)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x05, 0x01, 			//         Usage Page (Generic Desktop)
	0x26, 0xFF, 0x07,       //         Logical Maximum (2047)
	0x95, 0x01, 			//         Report Count (1)
	0x75, 0x10, 			//         Report Size (16)
	0x55, 0x0E, 			//         Unit Exponent (-2)
	0x65, 0x11, 			//         Unit (SI Lin: Length (cm))
	0x09, 0x30, 			//         Usage (X)
	0x35, 0x00, 			//         Physical Minimum (0)
	0x46, 0xB1, 0x08, 	    //         Physical Maximum (2225)
	0x81, 0x02, 		   	//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x46, 0xE3, 0x04, 	    //         Physical Maximum (1251)
	0x09, 0x31, 			//         Usage (Y)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0xC0,                   //     End Collection
	0xA1, 0x02, 			//     Collection (Logical)
	0x05, 0x0D, 			//         Usage Page (Digitizer)
	0x09, 0x42, 			//         Usage (Tip Switch)
	0x09, 0x32, 			//         Usage (In Range)
	0x15, 0x00, 			//         Logical Minimum (0)
	0x25, 0x01, 			//         Logical Maximum (1)
	0x95, 0x02, 			//         Report Count (2)
	0x75, 0x01, 			//         Report Size (1)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x95, 0x06, 			//         Report Count (6)
	0x81, 0x03, 			//         Input (Cnst,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x09, 0x51, 			//         Usage (Contact Identifier)
	0x95, 0x01, 			//         Report Count (1)
	0x75, 0x08, 			//         Report Size (8)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x05, 0x01, 			//         Usage Page (Generic Desktop)
	0x26, 0xFF, 0x07,       //         Logical Maximum (2047)
	0x95, 0x01, 			//         Report Count (1)
	0x75, 0x10, 			//         Report Size (16)
	0x55, 0x0E, 			//         Unit Exponent (-2)
	0x65, 0x11, 			//         Unit (SI Lin: Length (cm))
	0x09, 0x30, 			//         Usage (X)
	0x35, 0x00, 			//         Physical Minimum (0)
	0x46, 0xB1, 0x08, 	    //         Physical Maximum (2225)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x46, 0xE3, 0x04, 	    //         Physical Maximum (1251)
	0x09, 0x31, 			//         Usage (Y)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0xC0,                   //     End Collection
	0xA1, 0x02, 			//     Collection (Logical)
	0x05, 0x0D, 			//         Usage Page (Digitizer)
	0x09, 0x42, 			//         Usage (Tip Switch)
	0x09, 0x32, 			//         Usage (In Range)
	0x15, 0x00, 			//         Logical Minimum (0)
	0x25, 0x01, 			//         Logical Maximum (1)
	0x95, 0x02, 			//         Report Count (2)
	0x75, 0x01, 			//         Report Size (1)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x95, 0x06, 			//         Report Count (6)
	0x81, 0x03, 			//         Input (Cnst,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x09, 0x51, 			//         Usage (Contact Identifier)
	0x95, 0x01, 			//         Report Count (1)
	0x75, 0x08, 			//         Report Size (8)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x05, 0x01, 			//         Usage Page (Generic Desktop)
	0x26, 0xFF, 0x07,       //         Logical Maximum (2047)
	0x95, 0x01, 			//         Report Count (1)
	0x75, 0x10, 			//         Report Size (16)
	0x55, 0x0E, 			//         Unit Exponent (-2)
	0x65, 0x11, 			//         Unit (SI Lin: Length (cm))
	0x09, 0x30, 			//         Usage (X)
	0x35, 0x00, 			//         Physical Minimum (0)
	0x46, 0xB1, 0x08, 	    //         Physical Maximum (2225)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x46, 0xE3, 0x04, 	    //         Physical Maximum (1251)
	0x09, 0x31, 			//         Usage (Y)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0xC0,                   //     End Collection
	0xA1, 0x02, 			//     Collection (Logical)
	0x05, 0x0D, 			//         Usage Page (Digitizer)
	0x09, 0x42, 			//         Usage (Tip Switch)
	0x09, 0x32, 			//         Usage (In Range)
	0x15, 0x00, 			//         Logical Minimum (0)
	0x25, 0x01, 			//         Logical Maximum (1)
	0x95, 0x02, 			//         Report Count (2)
	0x75, 0x01, 			//         Report Size (1)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x95, 0x06, 			//         Report Count (6)
	0x81, 0x03, 			//         Input (Cnst,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x09, 0x51, 			//         Usage (Contact Identifier)
	0x95, 0x01, 			//         Report Count (1)
	0x75, 0x08, 			//         Report Size (8)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x05, 0x01, 			//         Usage Page (Generic Desktop)
	0x26, 0xFF, 0x07,       //         Logical Maximum (2047)
	0x95, 0x01, 			//         Report Count (1)
	0x75, 0x10, 			//         Report Size (16)
	0x55, 0x0E, 			//         Unit Exponent (-2)
	0x65, 0x11, 			//         Unit (SI Lin: Length (cm))
	0x09, 0x30, 			//         Usage (X)
	0x35, 0x00, 			//         Physical Minimum (0)
	0x46, 0xB1, 0x08, 	    //         Physical Maximum (2225)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x46, 0xE3, 0x04, 	    //         Physical Maximum (1251)
	0x09, 0x31, 			//         Usage (Y)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0xC0,                   //     End Collection
	0xA1, 0x02, 			//     Collection (Logical)
	0x05, 0x0D, 			//         Usage Page (Digitizer)
	0x09, 0x42, 			//         Usage (Tip Switch)
	0x09, 0x32, 			//         Usage (In Range)
	0x15, 0x00, 			//         Logical Minimum (0)
	0x25, 0x01, 			//         Logical Maximum (1)
	0x95, 0x02, 			//         Report Count (2)
	0x75, 0x01, 			//         Report Size (1)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x95, 0x06, 			//         Report Count (6)
	0x81, 0x03, 			//         Input (Cnst,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x09, 0x51, 			//         Usage (Contact Identifier)
	0x95, 0x01, 			//         Report Count (1)
	0x75, 0x08, 			//         Report Size (8)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x05, 0x01, 			//         Usage Page (Generic Desktop)
	0x26, 0xFF, 0x07,       //         Logical Maximum (2047)
	0x95, 0x01, 			//         Report Count (1)
	0x75, 0x10, 			//         Report Size (16)
	0x55, 0x0E, 			//         Unit Exponent (-2)
	0x65, 0x11, 			//         Unit (SI Lin: Length (cm))
	0x09, 0x30, 			//         Usage (X)
	0x35, 0x00, 			//         Physical Minimum (0)
	0x46, 0xB1, 0x08, 	    //         Physical Maximum (2225)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x46, 0xE3, 0x04, 	    //         Physical Maximum (1251)
	0x09, 0x31, 			//         Usage (Y)
	0x81, 0x02, 			//         Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0xC0,                   //   End Collection
	0x05, 0x0D, 			//     Usage Page (Digitizer)
	0x09, 0x54, 			//     Usage (Contact Count)
	0x95, 0x01, 			//     Report Count (1)
	0x75, 0x08, 			//     Report Size (8)
	0x25, 0x05, 			//     Logical Maximum (5)
	0x81, 0x02, 			//     Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x85, 0x02, 			//     Report ID (2)
	0x09, 0x55, 			//     Usage (Contact Count Maximum)
	0xB1, 0x02, 			//     Feature (Data,Var,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
	0xC0,                   // End Collection
	0x05, 0x0C, 			// Usage Page (Consumer Devices)
	0x09, 0x01, 			// Usage (Consumer Control)
	0xA1, 0x01, 			// Collection (Application)
	0x85, 0x04, 			//     Report ID (4)
	0x95, 0x01, 			//     Report Count (1)
	0x75, 0x10, 			//     Report Size (16)
	0x16, 0x00, 0x00, 	    //     Logical Minimum (0)
	0x26, 0x9C, 0x02, 	    //     Logical Maximum (668)
	0x05, 0x0C, 			//     Usage Page (Consumer Devices)
	0x1A, 0x00, 0x00, 	    //     Usage Minimum (Undefined)
	0x2A, 0x9C, 0x02, 	    //     Usage Maximum (AC Distribute Vertically)
	0x81, 0x00, 			//     Input (Data,Ary,Abs)
	0xC0,                   // End Collection
	0x05, 0x01, 			// Usage Page (Generic Desktop)
	0x09, 0x80, 			// Usage (System Control)
	0xA1, 0x01, 			// Collection (Application)
	0x85, 0x05, 			//     Report ID (5)
	0x75, 0x01, 			//     Report Size (1)
	0x95, 0x08, 			//     Report Count (8)
	0x15, 0x00, 			//     Logical Minimum (0)
	0x25, 0x01, 			//     Logical Maximum (1)
	0x09, 0x81, 			//     Usage (System Power Down)
	0x09, 0x82, 			//     Usage (System Sleep)
	0x09, 0x83, 			//     Usage (System Wake Up)
	0x09, 0x84, 			//     Usage (System Context Menu)
	0x09, 0x85, 			//     Usage (System Main Menu)
	0x09, 0x86, 			//     Usage (System App Menu)
	0x09, 0x87, 			//     Usage (System Menu Help)
	0x09, 0x88, 			//     Usage (System Menu Exit)
	0x81, 0x02, 			//     Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0xC0,                   // End Collection
};



///* USB Device mode configuration */
void USBDeviceCfg(void)
{
    USB_CTRL = 0x00;                                                           // Clear USB control register
    USB_CTRL &= ~bUC_HOST_MODE;                                                // This bit selects the device mode
    USB_CTRL |=  bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN;                    // USB device and internal pull-up enable, automatically return to NAK before interrupt flag is cleared, USB DMA enabled
    USB_DEV_AD = 0x00;                                                         // Device address initialization
    //     USB_CTRL |= bUC_LOW_SPEED;
    //     UDEV_CTRL |= bUD_LOW_SPEED;                                         // Select low speed 1.5M mode
    USB_CTRL &= ~bUC_LOW_SPEED;
    UDEV_CTRL &= ~bUD_LOW_SPEED;                                               // Select full speed 12M mode, the default mode
    UDEV_CTRL = bUD_PD_DIS;                                                    // Disable DP / DM pull-down resistor
    UDEV_CTRL |= bUD_PORT_EN;                                                  // Enable physical port
}


///* USB interrupt in Device mode initialization */
void USBDeviceIntCfg(void)
{
    USB_INT_EN |= bUIE_SUSPEND;                                                // Enable device suspend interrupt
    USB_INT_EN |= bUIE_TRANSFER;                                               // Enable USB transfer completion interrupt
    USB_INT_EN |= bUIE_BUS_RST;                                                // Enable device mode USB bus reset interrupt
    USB_INT_FG |= 0x1F;                                                        // Clear interrupt flag
    IE_USB = 1;                                                                // Enable USB interrupt
}

///* USB device mode endpoint configuration, simulation compatible HID device, in addition to endpoint 0 control transmission, also includes endpoint 2 batch upload */
void USBDeviceEndPointCfg(void)
{
    UEP2_DMA = (uint16_t) Ep2Buffer;                                             // Endpoint 2 IN data transfer address
    UEP2_3_MOD |= bUEP2_TX_EN | bUEP2_RX_EN;                                   //�˵�2���ͽ���ʹ��
    UEP2_3_MOD &= ~bUEP2_BUF_MOD;                                              //�˵�2�շ���64�ֽڻ�����
    UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;                 //�˵�2�Զ���תͬ����־λ��IN���񷵻�NAK��OUT����ACK                              // Endpoint 2 auto Toggle flag, IN trans return NAK



    UEP0_DMA = (uint16_t) Ep0Buffer;                                                      // Endpoint 0 data transfer address
    UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;                                 // Manual flip, OUT transaction returns ACK, IN transaction returns NAK
}


bool (* __xdata createHIDHandler)(USB_DigitizerReport_Data_t*);

void USB_setHIDCreateHandler(bool (* __xdata handler)(USB_DigitizerReport_Data_t*)) {
	createHIDHandler = handler;
}


void Enp2BulkIn(void)
{
	createHIDHandler(&DigitizerReportData);
	CreateReport(&DigitizerReport);

    memcpy( Ep2Buffer+MAX_PACKET_SIZE, &DigitizerReport, sizeof(USB_DigitizerReport_t));

    UEP2_T_LEN = sizeof(USB_DigitizerReport_t);
    UEP2_CTRL = (UEP2_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_ACK;
    while(( UEP2_CTRL & MASK_UEP_T_RES ) == UEP_T_RES_ACK);                  			// Waiting upload complete, avoid overwriting


}




void DeviceInterrupt(void) __interrupt (INT_NO_USB)
{
	uint16_t len;                                                               // USB receiving length
	uint8_t i;

	if(UIF_TRANSFER)		                                                    // USB transfer completion interrupt flag
	{
		switch(USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP))                   // Process transfer according to current endpoint number(MASK_UIS_ENDP) and current transaction token PID identifier(MASK_UIS_TOKEN)
		{                                                                       // (00xx xxxx & (0011 0000 | 0000 1111))
			case UIS_TOKEN_IN | 2:											    // Process IN(transmission) on Endpoint 2
			{
	            UEP2_T_LEN = 0;                                                    //Ԥʹ�÷��ͳ���һ��Ҫ���
//	            UEP2_CTRL ^= bUEP_T_TOG;                                          //����������Զ���ת����Ҫ�ֶ���ת
	            UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;           //Ĭ��Ӧ��NAK
				break;
			}
			case UIS_TOKEN_OUT | 2:											    // Process OUT(reception) on Endpoint 2
			{
				if (U_TOG_OK)                                                   // Reject unsynchronized packets
				{
	                len = USB_RX_LEN;                                           // Read the quantity of bytes of the data received by the current USB endpoint and write to len variable
	                for ( i = 0; i < len; i ++ )
	                {
	                    Ep2Buffer[MAX_PACKET_SIZE+i] = Ep2Buffer[i] ^ 0xFF;     // OUT data are changed to IN data (^ bitwise XOR)֤
	                }
	                UEP2_T_LEN = len;                                           // Write len value to register where Endpoint 2 transmittal data length is stored
	                UEP2_CTRL = (UEP2_CTRL & (~MASK_UEP_R_RES)) | UEP_T_RES_ACK;// Data ready, expected ACK
				}
				break;
			}
			case UIS_TOKEN_SETUP | 0:                                           // Process SETUP on Endpoint 0
			{
				len = USB_RX_LEN;												// Read the quantity of bytes of the data received by the current USB endpoint and write to len variable
				if(len == (sizeof(USB_SETUP_REQ)))                              // Check if received data lenght has 8 bytes of standard Setup packet
				{
//					SetupLen = UsbSetupBuf->wLengthL;                           // Save in global variable SetupLen number of bytes requested in Setup packet to transfer in data phase (response)
					SetupLen = ((((uint16_t)(UsbSetupBuf->wLengthH)) << 8) | (UsbSetupBuf->wLengthL)) ;
					len = 0;													// Set default length to 0
					SetupReq = UsbSetupBuf->bRequest;                           // Save in global variable SetupReq code of request from Setup packet
					if ((UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK) != USB_REQ_TYP_STANDARD) 	// Handle NOT standard device requests
	                {
						switch(SetupReq)
						{
							case HID_GET_REPORT:                                // Handle class specific GET_REPORT request (host receive report via control pipe)
							{
								pDescr = FeatureReport;
								len = sizeof(FeatureReport);
								break;
							}
							case HID_GET_IDLE:                                  // Handle class specific GET_IDLE request - ignore
							{
								break;
							}
							case HID_GET_PROTOCOL:                              // Handle class specific GET_PROTOCOL request - ignore
							{
								break;
							}
							case HID_SET_REPORT:                                // Handle class specific SET_REPORT request - ignore
							{
								break;
							}
							case HID_SET_IDLE:                                  // Handle class specific SET_IDLE request - ignore
							{
								break;
							}
							case HID_SET_PROTOCOL:                              // Handle class specific GET_PROTOCOL request - ignore
							{
								break;
	                		}
							default:											// Unknown class specific request
							{
								len = 0xFF;
								break;
							}
						}
						if(SetupLen > len)
						{
							SetupLen = len;                                     // Limitation of total length
						}
						len = SetupLen >= THIS_ENDP0_SIZE ? THIS_ENDP0_SIZE : SetupLen;   		// Set transmission length
						memcpy(Ep0Buffer,pDescr,len);                           // Load the transmission data
						SetupLen -= len;
						pDescr += len;
	                }
					else 															// Handle standard host to device requests
					{
						switch(SetupReq)										// Request code for standard host to device request type
						{
							case USB_GET_DESCRIPTOR:							// Handle standard host to device GET_DESCRIPTOR request
							{
								switch(UsbSetupBuf->wValueH)
								{
									case USB_DESCR_TYP_DEVICE:					// When host requests for Device Descriptor
										pDescr = DevDesc;
										len = sizeof(DevDesc);
										break;
									case USB_DESCR_TYP_CONFIG:					// When host requests for Configuration Descriptor
										pDescr = CfgDesc;
										len = sizeof(CfgDesc);
										break;
									case USB_DESCR_TYP_STRING:
	                                {
	                                    switch(UsbSetupBuf->wValueL)
	                                    {
	                                        case 1:
	                                            pDescr = MyManuInfo;
	                                            len = sizeof(MyManuInfo);
	                                            break;
	                                        case 2:
	                                            pDescr = MyProdInfo;
	                                            len = sizeof(MyProdInfo);
	                                            break;
	                                        case 0:
	                                            pDescr = MyLangDescr;
	                                            len = sizeof(MyLangDescr);
	                                            break;
	                                        default:
	                                            len = 0xFF;                               // ��֧�ֵ��ַ���������
	                                            break;
	                                    }
	                                    break;
	                                }
			                        case USB_DESCR_TYP_REPORT:					// When host requests for HID Report Descriptor
//			                            printf("Report Deskyryptor request\n\r");
			                        	pDescr = HIDRepDesc;
			                            len = sizeof(HIDRepDesc);
			                            break;
			                        default:
			                            len = 0xFF;                             // Unsupported descriptor type or error
			                            break;
								}
								if(SetupLen > len)
								{
									SetupLen = len;			                    // If requested data length to be sent in response is greater than actual descriptor size - limit it to decsriptor size
								}
								len = SetupLen >= THIS_ENDP0_SIZE ? THIS_ENDP0_SIZE : SetupLen;
									memcpy(Ep0Buffer,pDescr,len);
									SetupLen -= len;
									pDescr +=len;
								break;
							} /* case USB_GET_DESCRIPTOR */
							case USB_SET_ADDRESS:                               // Handle standard host to device SET_ADDRESS request
								SetupLen = UsbSetupBuf->wValueL;                // Save the device address but why in SetupLen?
								break;
							case USB_GET_CONFIGURATION:							// Handle standard host to device GET_CONFIGURATION request
								Ep0Buffer[0] = UsbConfig;
								if(SetupLen >= 1)
								{
									len = 1;
								}
								break;
							case USB_SET_CONFIGURATION:							// Handle standard host to device GET_CONFIGURATION request
								UsbConfig = UsbSetupBuf->wValueL;
								if(UsbConfig)
								{
									Ready = 1;
								}
								break;
							case USB_GET_INTERFACE:								// Handle standard host to device GET_INTERFACE request
								break;
							case USB_CLEAR_FEATURE:								// Handle standard host to device CLEAR_FEATURE request
		                        if((UsbSetupBuf->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_ENDP)  // Checks if a request Recipient is Endpoint
		                        {
		                            switch(UsbSetupBuf->wIndexL)				// Clear Endpoint 1 IN and Endpoint 2 IN and OUT
		                            {
		                            case 0x82:
		                                UEP2_CTRL = (UEP2_CTRL & ~( bUEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_NAK;
		                                break;
		                            case 0x81:
		                                UEP1_CTRL = (UEP1_CTRL & ~( bUEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_NAK;
		                                break;
		                            case 0x02:
		                                UEP2_CTRL = (UEP2_CTRL & ~( bUEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_ACK;
		                                break;
		                            default:
		                                len = 0xFF;                             // No such endpoint - error
		                                break;
		                            }
		                        }
		                        else
		                        {
		                            len = 0xFF;                                 // Recipient different than endpoint
		                        }
		                        break;
							case USB_SET_FEATURE:								// Handle standard host to device SET_FEATURE request
		                        if((UsbSetupBuf->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_DEVICE)  // Checks if a request Recipient is Device
		                        {
		                            if((((uint16_t)UsbSetupBuf->wValueH << 8) | UsbSetupBuf->wValueL) == 0x01)
		                            {
		                                if( CfgDesc[7] & 0x20 ) 				// Only if Remote Wakeup bit in bmAttributes has been selected in Configuration Descriptor
		                                {
		                                    /* Awake the device */
		                                }
		                                else
		                                {
		                                    len = 0xFF;                         // Error routing - Rmote Wakeup not enabled in Configuration Descriptor
		                                }
		                            }
		                            else
		                            {
		                                len = 0xFF;                             // Error routing - unknown feature to set
		                            }
		                        }
		                        else if((UsbSetupBuf->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_ENDP)  // Checks if a request Recipient is Endpoint
		                        {
		                            if((((uint16_t)UsbSetupBuf->wValueH << 8) | UsbSetupBuf->wValueL) == 0x00)   // Checks if the command is 0x00(ENDPOINT_HALT)
		                            {
		                                switch(((uint16_t)UsbSetupBuf->wIndexH << 8) | UsbSetupBuf->wIndexL)
		                                {
		                                case 0x82:
		                                    UEP2_CTRL = (UEP2_CTRL & (~bUEP_T_TOG)) | UEP_T_RES_STALL;			// Endpoint 2 IN STALL
		                                    break;
		                                case 0x02:
		                                    UEP2_CTRL = (UEP2_CTRL & (~bUEP_R_TOG)) | UEP_R_RES_STALL;          // Endpoint 2 OUT STALL
		                                    break;
		                                case 0x81:
		                                    UEP1_CTRL = (UEP1_CTRL & (~bUEP_T_TOG)) | UEP_T_RES_STALL;			// Endpoint 1 IN STALL
		                                    break;
		                                default:
		                                    len = 0xFF;                         // No such endpoint - error
		                                    break;
		                                }
		                            }
		                            else
		                            {
		                                len = 0xFF;                             // Not supported command for Endpoint recipient
		                            }
		                        }
		                        else											// We support only SET_FEATURE request for Device and Endpoint Recipient
		                        {
		                            len = 0xFF;
		                        }
		                        break;
							case USB_GET_STATUS:								// Handle standard device GET_STATUS request. Device must return 2 bytes
		                        Ep0Buffer[0] = 0x00;							// Device is bus powered
		                        Ep0Buffer[1] = 0x00;							// Device has remote Wake-up disabled
		                        if (SetupLen >= 2)
		                        {
		                            len = 2;									// Device is returning only 2 bytes, not more
		                        }
		                        else
		                        {
		                            len = SetupLen;
		                        }
		                        break;
							default:
								len = 0xFF;
								break;

						}
					}
				} // end if(len == (sizeof(USB_SETUP_REQ)))
				else															//Setup packet length error
				{
					len = 0xFF;
				}

	            if(len == 0xFF)
	            {
	                SetupReq = 0xFF;
	                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;//STALL
	            }
	            else if(len <= THIS_ENDP0_SIZE)//上传数据或者状态阶段返回0长度包
	            {
	                UEP0_T_LEN = len;
	                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1，返回应答ACK
	            }
	            else
	            {
	                UEP0_T_LEN = 0;  //虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
	                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1,返回应答ACK
	            }
	            break;
			}
			case UIS_TOKEN_IN | 0:
            	switch(SetupReq)
            	{
				case USB_GET_DESCRIPTOR:
				case HID_GET_REPORT:
					len = SetupLen >= THIS_ENDP0_SIZE ? THIS_ENDP0_SIZE : SetupLen;                                 //本次传输长度
//					len = SetupLen >= THIS_ENDP0_SIZE ? DEFAULT_ENDP0_SIZE : SetupLen;
					memcpy( Ep0Buffer, pDescr, len );                                   //加载上传数据
					SetupLen -= len;
//					printf("Setup len in: %d, len: %d\n\r", SetupLen, len);
					pDescr += len;
					UEP0_T_LEN = len;
					UEP0_CTRL ^= bUEP_T_TOG;                                             //同步标志位翻转
					break;
				case USB_SET_ADDRESS:
					USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
					UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
					break;
				default:
					UEP0_T_LEN = 0;                                                      //状态阶段完成中断或者是强制上传0长度数据包结束控制传输
					UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
					break;
				}
				break;
            case UIS_TOKEN_OUT | 0:  // endpoint0 OUT
				len = USB_RX_LEN;
				if(SetupReq == 0x09)
				{
					if(Ep0Buffer[0])
					{
//						printf("Light on Num Lock LED!\n");
					}
					else if(Ep0Buffer[0] == 0)
					{
//						printf("Light off Num Lock LED!\n");
					}
				}
				UEP0_CTRL ^= bUEP_R_TOG;                                     //ͬ����־λ��ת
				break;
            default:
            	break;
			}
			UIF_TRANSFER = 0;
		}
		if(UIF_BUS_RST)                                                         // Interrupt flag in USB device mode triggered by USB bus reset event
		{
			UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;                          // Configures endpoint 0 control register to acknowledge reception (UEP_R_RES_ACK) and indicate no data to be transmitted (UEP_T_RES_NAK)
			UEP1_CTRL = bUEP_AUTO_TOG | UEP_R_RES_NAK;                          // Configures endpoint 1 control register to enable automatic toggle (bUEP_AUTO_TOG) and indicate no reception (UEP_R_RES_NAK)
			UEP2_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;          // Configures endpoint 2 control register to enable automatic toggle, acknowledge reception, and indicate no data to be transmitted
			USB_DEV_AD = 0x00;                                                  // Sets the USB device address to 0
			UIF_SUSPEND = 0;                                                    // Clears the USB suspend flag
			UIF_TRANSFER = 0;                                                   // Clears the USB transfer flag
	        Ready = 0;
			UIF_BUS_RST = 0;                                                    // Clear this interrupt flag
		}
		if (UIF_SUSPEND)                                                        // USB Suspended and Awake interrupt
		{
			UIF_SUSPEND = 0;                                                    // Clear this interrupt flag
			if ( USB_MIS_ST & bUMS_SUSPEND )                                    // Checking if suspend flag is present
			{
//				while ( XBUS_AUX & bUART0_TX )
//				{
//					;                                                           // Wait for UART0 transmission to be completed
//				}
//				SAFE_MOD = 0x55;                                                // Send 0x55 then 0xAA to enter safe mode
//				SAFE_MOD = 0xAA;
//				WAKE_CTRL = bWAK_BY_USB | bWAK_RXD0_LO | bWAK_RXD1_LO;          // Enable waking up by action on USB Bus or RXD0 low level
//				PCON |= PD;                                                     // Enter sleep mode bit, auto cleared by hardware after wake-up
//				SAFE_MOD = 0x55;                                                // Send 0x55 then 0xAA to enter safe mode
//				SAFE_MOD = 0xAA;
//				WAKE_CTRL = 0x00;                                               // Disable previously selected wake-up hardware sources
			}
		}
		else {                                                                  // Unexpected interrupt
			USB_INT_FG = 0xFF;                                                  // Reset the interrupt
		}
}
