#include <stdio.h>
#include "ch554.h"
#include "delay.h"
#include "calibrator.h"
#include "ch554_usb.h"

#include "gt911.h"
#include "gt911_reg.h"
#include "gt911_if.h"
#include "gt911_structs.h"

#ifdef DEBUG
#include "debug.h"
#endif // DEBUG


void INT0_EnableInterrupt(void);
void INT1_EnableInterrupt(void);
void INT_GlobalEnable(void);

// Global variables used by Callback function
volatile uint8_t touchContacts = 0;
volatile GTPoint_t *touchPoints;


void INT1_FallingEdge_ISR(void) __interrupt (INT_NO_INT1)
{
	printf("Calibrator ISR entered\n\r");
    IE1 = 0;
    Calibrator_tic();
}



// Callback function called when INT0 occurs
void __xdata handleTouch(int8_t contacts, GTPoint_t *points) __reentrant
{
	if(!Calibrator_adjust(contacts, points))
	{
		return; // calibration in progress
	}

	touchContacts = contacts;
	touchPoints = points;

/* DEBUG */
//	if(touchContacts == 1)
//		{
//
//			printf("x mian: %d ",touchPoints[0].x);
//			printf("y: %d",touchPoints[0].y);
//			printf("\n\r");
//		}
//		else if(touchContacts == 2)
//		{
//
//			printf("x1: %d ",touchPoints[0].x);
//			printf("y1: %d ",touchPoints[0].y);
//
//			printf("x2: %d ",touchPoints[1].x);
//			printf("y2: %d ",touchPoints[1].y);
//			printf("\n\r");
//		}
//		else if(touchContacts == 3)
//		{
//
//			printf("x1: %d ",touchPoints[0].x);
//			printf("y1: %d ",touchPoints[0].y);
//
//			printf("x2: %d ",touchPoints[1].x);
//			printf("y2: %d ",touchPoints[1].y);
//
//			printf("x3: %d ",touchPoints[2].x);
//			printf("y3: %d ",touchPoints[2].y);
//			printf("\n\r");
//		}
/* DEBUG */
}

void (*__xdata Handler)(int8_t, GTPoint_t*) __reentrant = handleTouch;




bool __xdata createHIDReport(USB_DigitizerReport_Data_t* report) {
	GTPoint_t *points = touchPoints;
	uint8_t contacts = touchContacts;

	report->Finger0_ContactIdentifier = 0;
	report->Finger0_InRange = 1;
//	report->data = 0x01;
	if(contacts > 0) {
//		report->data = 0x03;
		report->Finger0_TipSwitch = 1;
		report->Finger0_X = points[0].x;
		report->Finger0_Y = points[0].y;
	}

	report->Finger1_ContactIdentifier = 1;
	report->Finger1_InRange = 1;
	if(contacts > 1) {
		report->Finger1_TipSwitch = 1;
		report->Finger1_X = points[1].x;
		report->Finger1_Y = points[1].y;
	}

	report->Finger2_ContactIdentifier = 2;
	report->Finger2_InRange = 1;
	if(contacts > 2) {
		report->Finger2_TipSwitch = 1;
		report->Finger2_X = points[2].x;
		report->Finger2_Y = points[2].y;
	}

	report->Finger3_ContactIdentifier = 3;
	report->Finger3_InRange = 1;
	if(contacts > 3) {
		report->Finger3_TipSwitch = 1;
		report->Finger3_X = points[3].x;
		report->Finger3_Y = points[3].y;
	}

	report->Finger4_ContactIdentifier = 4;
	report->Finger4_InRange = 1;
	if(contacts > 4) {
		report->Finger4_TipSwitch = 1;
		report->Finger4_X = points[4].x;
		report->Finger4_Y = points[4].y;
	}

	report->ContactCount = contacts;
	return contacts > 0;
}

bool (*__xdata reportHandler)(USB_DigitizerReport_Data_t*) = createHIDReport;



int main()
{
	/* Register Callbacks */
	Goodix_setHandler(Handler);
	INT0_EnableInterrupt();
	INT1_EnableInterrupt();

	/* Set the system frequency nad enable UART0 with baudrate 9600 */
	CfgFsys();
#ifdef DEBUG
	UART0_Init();
#endif // DEBUG


	Calibrator_init();


	/*USB init */
	USB_setHIDCreateHandler(reportHandler);
    USBDeviceCfg();
    USBDeviceEndPointCfg();
    USBDeviceIntCfg();

    UEP0_T_LEN = 0;                                                         	// Pre-use send length must be cleared
    UEP2_T_LEN = 0;

    INT_GlobalEnable();
	while(1)
	{
		Goodix_loop();
		if(touchContacts > 0)
		{
			Enp2BulkIn();
		}
	}
}



void INT0_EnableInterrupt(void)
{
    EX0 = 1;                // enable external interrupt INT0
    IT0 = 1;                // falling edge triggers interrupt

    /* High impedance input mode, interrupt 0 pin has no pull-up resistor */
    P3_MOD_OC &= ~(1 << 2);
    P3_DIR_PU &= ~(1 << 2);
}

void INT1_EnableInterrupt(void)
{
    EX1 = 1;                // enable external interrupt INT0
    IT1 = 1;                // falling edge triggers interrupt

    /* High impedance input mode, interrupt 1 pin has no pull-up resistor */
    P3_MOD_OC &= ~(1 << 3);
    P3_DIR_PU &= ~(1 << 3);
}

void INT_GlobalEnable(void)
{
	EA = 1;                 // enable global interrupts
}
