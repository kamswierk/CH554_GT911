#include "debug.h"
#include "ch554.h"



int putchar(int c)
{
    SBUF = c; 		// @suppress("Symbol is not resolved")
    while (!TI); 	// @suppress("Symbol is not resolved")
    TI = 0;  		// Clear transmit interrupt flag // @suppress("Symbol is not resolved")

    return c;
}


void UART0_Init(void)
{
	SCON = 0x50;  	// 8-bit UART, enable UART0 // @suppress("Symbol is not resolved")
	PCON = 0x10; 	// @suppress("Symbol is not resolved")
    RCLK = 0;     	// UART0 receive clock // @suppress("Symbol is not resolved")
    TCLK = 0;     	// UART0 transmit clock // @suppress("Symbol is not resolved")

    /* Configure Timer1 for baud rate generation */
    T2MOD = 0xA0; 	// @suppress("Symbol is not resolved")
    TMOD |= bT1_M1;  	// Set Timer1 in Mode 2 // @suppress("Symbol is not resolved")
    //TH1 = 256 - (24000000 / 32 / 4 / 9600);  // Calculate TH1 for 9600 baud rate
    // 0xB2 = 2400
    // 0xEC = 9600
    TH1 = 0xEC; 	//Write calculated TH1 value in HEX // @suppress("Symbol is not resolved")
    TR1 = 1;  		// Start Timer1 // @suppress("Symbol is not resolved")

}
