#ifndef CORE_INC_DATAFLASHY_C_
#define CORE_INC_DATAFLASHY_C_


uint8_t WriteDataFlash(uint8_t Addr,uint8_t *buf,uint8_t len);

uint8_t ReadDataFlash(uint8_t Addr,uint8_t len,uint8_t *buf);


#endif /* CORE_INC_DATAFLASHY_C_ */
