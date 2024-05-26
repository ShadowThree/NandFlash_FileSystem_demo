#ifndef NAND_FLASH_FS_INTERFACE_H
#define NAND_FLASH_FS_INTERFACE_H

#include <stdint.h>
#include <stddef.h>
#include "rl_fs.h"

void init_filesystem(uint8_t forceFormat);
void show_dir(char *mask);
void make_dir(char *dir);
size_t write_file(char* name, uint8_t* buf, size_t num);
size_t read_file(char* name, uint8_t* buf, size_t num);

#endif // NAND_FLASH_FS_INTERFACE_H
