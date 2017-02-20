#ifndef __HALW25X16_H
#define __HALW25X16_H


#define W25_PAGE_LEN    256
#define W25_SECTOR_LEN	0x1000
#define W25_BLOCK_LEN	0x40000
#define W25_FLASH_LEN		0x200000 //2M


void drv25Config(void);
void drv25Init(void);
void drv25WriteDisable(void);
void drv25WriteEnable(void);
int8u drv25ReadStatus(void);
void drv25WriteStatus(int8u state);
void drv25EraseChip(void);
void drv25EraseSector(int32u sector);
void drv25EraseBlock(int32u block);
void drvSST25GetID(int8u* pMId,int16u* pDId);
void drvW25GetID(int8u* pMId,int16u* pDId);
void drvW25ReadBytes(int32u addr,void* pDest,int32u len);
void drvW25WriteBytes(int32u addr,const void* pSrc,int32u len);
void drvSST25WriteBytes(int32u addr,const void* p_data,int32u len);






#endif

