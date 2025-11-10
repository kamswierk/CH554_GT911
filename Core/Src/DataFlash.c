#include <stdint.h>
#include "ch554.h"
#include "DataFlash.h"


uint8_t WriteDataFlash(uint8_t Addr,uint8_t *buf,uint8_t len)
{
    uint8_t i;
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;
    GLOBAL_CFG |= bDATA_WE;
    SAFE_MOD = 0;
		ROM_ADDR_H = DATA_FLASH_ADDR >> 8;
    Addr <<= 1;
    for(i=0;i<len;i++)
	  {
        ROM_ADDR_L = Addr + i*2;
        ROM_DATA_L = *(buf+i);			
        if ( ROM_STATUS & bROM_ADDR_OK )
        {
           ROM_CTRL = ROM_CMD_WRITE;
        }
        if((ROM_STATUS ^ bROM_ADDR_OK) > 0) return i;                          // x00=success,  0x02=unknown command(bROM_CMD_ERR)
	  }
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;
    GLOBAL_CFG &= ~bDATA_WE;
    SAFE_MOD = 0;
    return i;		
}


uint8_t ReadDataFlash(uint8_t Addr,uint8_t len,uint8_t *buf)
{
    uint8_t i;
    ROM_ADDR_H = DATA_FLASH_ADDR >> 8;
    Addr <<= 1;
    for(i=0;i<len;i++)
    {
	  ROM_ADDR_L = Addr + i*2;
	  ROM_CTRL = ROM_CMD_READ;
//     if ( ROM_STATUS & bROM_CMD_ERR ) return( 0xFF );                        // unknown command
	  *(buf+i) = ROM_DATA_L;
	}
    return i;
}
