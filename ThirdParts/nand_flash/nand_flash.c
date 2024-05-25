#include "nand_flash.h"

NAND_FLASH_STA_t nand_flash_erase_block(uint32_t addr)
{
	addr -= NAND_FLASH_ADDR;
	NAND_AddressTypeDef nandAddr = {.Page = 0, .Block = addr / BLOCK_SIZE, .Plane = 0};
	HAL_StatusTypeDef hal_sta = HAL_NAND_Erase_Block(&hnand1, &nandAddr);
	if(hal_sta == HAL_OK) {
		NAND_FLASH_DBG("nand flash erase block OK\n");
		return NAND_FLASH_STA_OK;
	} else {
		NAND_FLASH_ERR("nand flash erase block err[%d]\n", hal_sta);
		return NAND_FLASH_ERASE_BLOCK_ERR;
	}
}

NAND_FLASH_STA_t nand_flash_erase_chip(void)
{
	NAND_AddressTypeDef nandAddr = {.Page = 0, .Block = 0, .Plane = 0};
	for(uint16_t i = 0; i < BLOCK_NUM; i++) {
		nandAddr.Block = i;
		HAL_StatusTypeDef hal_sta = HAL_NAND_Erase_Block(&hnand1, &nandAddr);
		if(hal_sta != HAL_OK) {
			NAND_FLASH_ERR("nand flash erase chip at block[%d] err[%d]\n", i, hal_sta);
			return NAND_FLASH_ERASE_CHIP_ERR;
		}
	}
	NAND_FLASH_DBG("nand flash erase chip OK\n");
	return NAND_FLASH_STA_OK;
}

NAND_FLASH_STA_t nand_flash_read_page(uint32_t addr, uint8_t* buf)
{
	addr -= NAND_FLASH_ADDR;
	NAND_AddressTypeDef nandAddr = {.Page = addr % BLOCK_SIZE / PAGE_SIZE, .Block = addr / BLOCK_SIZE, .Plane = 0};
	HAL_StatusTypeDef hal_sta = HAL_NAND_Read_Page_8b(&hnand1, &nandAddr, buf, 1);
	if(hal_sta == HAL_OK) {
		NAND_FLASH_DBG("nand flash read OK");
		
		#if 1		// read data
		for(uint16_t i = 0; i < PAGE_SIZE; i++) {
			if(i / 16 >= 10) {	// the number of lines to print
				break;
			}
			if(i % 16 == 0) {
				NAND_FLASH_DBG("\n\t%03d:", i / 16 + 1);
				HAL_Delay(20);
			}
			NAND_FLASH_DBG(" %02x", buf[i]);
		}
		#endif
		
		NAND_FLASH_DBG("\n");
		return NAND_FLASH_STA_OK;
	} else {
		NAND_FLASH_ERR("nand flash read err[%d]\n", hal_sta);
		return NAND_FLASH_READ_ERR;
	}
}

NAND_FLASH_STA_t nand_flash_write_page(uint32_t addr, uint8_t* buf)
{
	addr -= NAND_FLASH_ADDR;
	NAND_AddressTypeDef nandAddr = {.Page = addr % BLOCK_SIZE / PAGE_SIZE, .Block = addr / BLOCK_SIZE, .Plane = 0};
	HAL_StatusTypeDef hal_sta = HAL_NAND_Write_Page_8b(&hnand1, &nandAddr, buf, 1);
	if(hal_sta == HAL_OK) {
		NAND_FLASH_DBG("nand flash write OK\n");
		return NAND_FLASH_STA_OK;
	} else {
		NAND_FLASH_ERR("nand flash write err[%d]\n", hal_sta);
		return NAND_FLASH_WRITE_ERR;
	}
}

#define TEST_NUM    97
NAND_FLASH_STA_t NAND_FLASH_test(void)
{
    HAL_StatusTypeDef hal_sta = HAL_OK;
    NAND_IDTypeDef nand_id = {0};
    uint8_t buf[PAGE_SIZE] = {0};
		
		NAND_FLASH_DBG("\n*********** NAND_FLASH_test start **********\n");
    
		hal_sta = HAL_NAND_Reset(&hnand1);
    if(hal_sta == HAL_OK) {
        NAND_FLASH_DBG("nand flash reset OK\n");
    } else {
        NAND_FLASH_ERR("nand flash reset err[%d]\n", hal_sta);
        return NAND_FLASH_RESET_ERR;
    }
    
    // read ID (0xADF1001D for H27U1G8F2BTR)    // 0xADDC9095 for H27U4G8F2ETR
    hal_sta = HAL_NAND_Read_ID(&hnand1, &nand_id);
    if(hal_sta == HAL_OK) {
        NAND_FLASH_DBG("nand flash id[0x%02x%02x%02x%02x]\n", nand_id.Maker_Id, nand_id.Device_Id, nand_id.Third_Id, nand_id.Fourth_Id);
    } else {
        NAND_FLASH_ERR("read nand flash id err[%d]\n", hal_sta);
        return NAND_FLASH_READ_ID_ERR;
    }
    
		uint32_t addr = NAND_FLASH_ADDR + BLOCK_SIZE * (BLOCK_NUM - 1);		// last block
		NAND_FLASH_DBG("nand flash test at addr[0x%08x]\n", addr);
    nand_flash_erase_block(addr);
		nand_flash_read_page(addr, buf);
		
		// set data
		for(uint16_t i = 0; i < PAGE_SIZE; i++) {
        buf[i] = i % TEST_NUM;
    }
		
		nand_flash_write_page(addr, buf);
		nand_flash_read_page(addr, buf);
		
		// compare data
		for(uint16_t i = 0; i < PAGE_SIZE; i++) {
        if(buf[i] != i % TEST_NUM) {
					NAND_FLASH_ERR("nand flash data check err at[%d] (0x%02x 0x%02x)\n", i, i % TEST_NUM, buf[i]);
					return NAND_FLASH_CHECK_ERR;
				}
    }
		NAND_FLASH_DBG("nand flash data check OK\n");
		NAND_FLASH_DBG("*********** NAND_FLASH_test stop **********\n");
		return NAND_FLASH_STA_OK;
}

static const uint8_t nand_flash_test_data[PAGE_SIZE] __attribute__((section("ExtFlashSec"))) __attribute__((aligned(4))) = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x3F,
	0xF7, 0x5A
};

// Because the NandFlash NOT support access by memory map, we have no way to check the data.
// So this function will always return successful; Need check the data by yourself!!!
NAND_FLASH_STA_t NAND_FLASH_algorithm_result_test(void)
{
		NAND_FLASH_DBG("\n*********** NAND_FLASH_algorithm_result_test start **********\n");
    LOG_DBG("extFlash data addr[0x%08x]\n", nand_flash_test_data);
    uint8_t nand_read_data[PAGE_SIZE] = {0};
    nand_flash_read_page((uint32_t)nand_flash_test_data, nand_read_data);
		NAND_FLASH_DBG("*********** NAND_FLASH_algorithm_result_test stop **********\n");
		return NAND_FLASH_STA_OK;
}
