#include "flash_util.h"
#include "string.h"


void Flash_Erase(uint32_t Add) {
	
  uint32_t PageError = 0;
		/* Unlock the Flash to enable the flash control register */
	HAL_FLASH_Unlock();

  HAL_StatusTypeDef status;
  FLASH_EraseInitTypeDef eraseinitstruct;

 //__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
 //__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_PGSERR | FLASH_FLAG_PROGERR | FLASH_FLAG_BSY);
 
   /* Get the number of sector to erase from 1st sector*/
  eraseinitstruct.Banks = Flash_Get_Bank(Add);//获取擦除地址所在的堆
  eraseinitstruct.TypeErase = FLASH_TYPEERASE_PAGES;//按页擦除
  eraseinitstruct.Page = Flash_Get_Page(Add);//获取页位置
  eraseinitstruct.NbPages = 1;//擦1页
  status = HAL_FLASHEx_Erase(&eraseinitstruct, &PageError);
		HAL_FLASH_Lock();
  if (status != HAL_OK)
  {
    Flash_ErrorHandle();
  }

}

void Flash_Write_Uint8_Array(uint8_t *src, uint32_t dest_addr, uint32_t Len)
{
  uint32_t i = 0, loca1 = 0;
	//addrdr = (Flash_Get_Page(dest_addr)*FLASH_PAGE_SIZE+0x08000000);
	//memcpy(flash_temp,(void*)(Flash_Get_Page(dest_addr)*FLASH_PAGE_SIZE+0x08000000),sizeof(flash_temp));
	Flash_Erase(dest_addr);//写数据的地方不能有数据，得先擦除
 	HAL_FLASH_Unlock();
	//memcpy((flash_temp+ dest_addr-(Flash_Get_Page(dest_addr)*FLASH_PAGE_SIZE+0x08000000)),src,Len);
	
  /* Clear OPTVERR bit set on virgin samples */
  //__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_PGSERR | FLASH_FLAG_PROGERR);
	//__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
	
  for(i = 0; i < Len; i += 8)
  {
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t)(dest_addr+i),        *(uint64_t*)(src+i)) == HAL_OK)//*(uint64_t*)(src+i)表明一字节src的物理地址必须是连续的
    {
     /* Check the written value */
      if(*(uint64_t *)(src + i) != *(uint64_t*)(dest_addr+i))
      {
            Flash_ErrorHandle();
      }
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
        Flash_ErrorHandle();
    }
  }
	HAL_FLASH_Lock();
	
}

void Flash_Write_Float(float value,uint32_t dest_addr) {
	
	uint8_t float_to_char[4];
	memcpy(float_to_char,&value,4);
	Flash_Write_Uint8_Array(float_to_char,dest_addr,4);
}

void Flash_Read_Float(float *value,uint32_t dest_addr) {
	
	*value = *((float*)(dest_addr));
}

uint32_t Flash_Get_Page(uint32_t Addr)
{
  uint32_t page = 0;
 
  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }
 
  return page;
}
 
/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
uint32_t Flash_Get_Bank(uint32_t Addr)
{
  uint32_t bank = 0;
 
//  if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0)
//  {
//    /* No Bank swap */
//    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
//    {
//      bank = FLASH_BANK_1;
//    }
//    else
//    {
//      bank = FLASH_BANK_2;
//    }
//  }
//  else
//  {
//    /* Bank swap */
//    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
//    {
//      bank = FLASH_BANK_2;
//    }
//    else
//    {
//      bank = FLASH_BANK_1;
//    }
//  }
// 
  return bank;
    return FLASH_BANK_1; //G431只有FLASH_BANK_1
}

void Flash_ErrorHandle(void) {
	
	while(1) {
		
	}
	
}

 