/**
 ***** HOW TO USE *****
 
 #include "nand_flash.h"
 
 	NAND_FLASH_STA_t nand_sta;
	nand_sta = NAND_FLASH_test();
	if(NAND_FLASH_STA_OK != nand_sta) {
		LOG_ERR("NAND_FLASH_test err[%d]\n", nand_sta);
	}
	NAND_FLASH_algorithm_result_test();

 */
#ifndef __NAND_FLASH_H__
#define __NAND_FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "fmc.h"

#define NAND_FLASH_LOG_EN   1
#if NAND_FLASH_LOG_EN
    #include "dbger.h"
    #define NAND_FLASH_DBG(fmt, ...)        LOG_DBG(fmt, ##__VA_ARGS__)
    #define NAND_FLASH_ERR(fmt, ...)        LOG_ERR(fmt, ##__VA_ARGS__)
#else
    #define NAND_FLASH_DBG(fmt, ...)
    #define NAND_FLASH_ERR(fmt, ...)
#endif

// H27U1G8F2BTR params
#define PAGE_SIZE       (2048)              // 2KB
#define BLOCK_SIZE      (2048 * 64)         // 2KB * 64pages
#define BLOCK_NUM       (4096)              // totally 1024 blocks
#define PLANE_SIZE      (2048 * 64 * 4096)  // 512MB
#define PLANE_NUM    		(1)

// APOLLO
#define NAND_FLASH_ADDR (0x80000000)

typedef enum {
    NAND_FLASH_STA_OK = 0,
    NAND_FLASH_RESET_ERR,
    NAND_FLASH_READ_ID_ERR,
    NAND_FLASH_ERASE_BLOCK_ERR,
		NAND_FLASH_ERASE_CHIP_ERR,
    NAND_FLASH_READ_ERR,
    NAND_FLASH_WRITE_ERR,
    NAND_FLASH_CHECK_ERR,
    NAND_FLASH_ALGORITHM_RESULT_ERR
} NAND_FLASH_STA_t;

NAND_FLASH_STA_t nand_flash_erase_block(uint32_t addr);
NAND_FLASH_STA_t nand_flash_erase_chip(void);
NAND_FLASH_STA_t nand_flash_read_page(uint32_t addr, uint8_t* buf);
NAND_FLASH_STA_t nand_flash_write_page(uint32_t addr, uint8_t* buf);

NAND_FLASH_STA_t NAND_FLASH_test(void);
NAND_FLASH_STA_t NAND_FLASH_algorithm_result_test(void);

#ifdef __cplusplus
}
#endif

#endif  /* __NAND_FLASH_H__ */
