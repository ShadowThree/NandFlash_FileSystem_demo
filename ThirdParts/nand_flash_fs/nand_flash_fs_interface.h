#ifndef NAND_FLASH_FS_INTERFACE_H
#define NAND_FLASH_FS_INTERFACE_H

#include <stdint.h>
#include <stddef.h>

void init_filesystem (void);
void show_dir(char *par);
//void write_file(char *par);
size_t write_file(char* name, uint8_t* buf, size_t num);
size_t read_file(char* name, uint8_t* buf, size_t num);
//void read_file(char *par);
//void formatFlash(uint8_t force);
//void showCapacity(void);
#endif // NAND_FLASH_FS_INTERFACE_H
