#include "nand_flash_fs_interface.h"
#include "rl_fs.h"
#include "stdio.h"
#include "cmsis_os2.h"

#define NAND_FLASH_FS_DEBUG 1
#if NAND_FLASH_FS_DEBUG
#include "dbger.h"
#define FS_DBG(fmt, ...) LOG_DBG(fmt, ##__VA_ARGS__)
#define FS_ERR(fmt, ...) LOG_ERR(fmt, ##__VA_ARGS__)
#else
#define FS_DBG(fmt, ...)
#define FS_ERR(fmt, ...)
#endif

static void cmd_format(char *par);

void init_filesystem (void)
{
  fsStatus stat;
  /* Initialize and mount drive "M0" */
  stat = finit ("N0:");
  if (stat == fsOK) {
    stat = fmount ("N0:");
    if (stat == fsOK) {
      FS_DBG("Drive N0 ready!\n");
    }
    else if (stat == fsNoFileSystem) {
      /* Format the drive */
      FS_ERR("Drive N0 not formatted!\n");
      cmd_format("N0:");
    }
    else {
      FS_ERR("Drive N0 mount failed with error code %d\n", stat);
    }
  }
  else {
    FS_ERR("Drive N0 initialization failed!\n");
  }
}

size_t write_file(char* name, uint8_t* buf, size_t num)
{
	FILE* f;
	
	f = fopen(name, "w");
	if(f == NULL) {
		LOG_ERR("open \"%s\" err\n", name);
		return 0;
	}
	FS_DBG("write file %s\n", name);
	for(size_t i = 0; i < num; i++) {
		if(EOF == fputc(buf[i], f)) {
			fclose(f);
			FS_ERR("write \"%s\" err at byte[%d]\n", name, i);
			return i;
		}
	}
	fclose(f);
	return num;
}

size_t read_file(char* name, uint8_t* buf, size_t num)
{
	FILE* f;
	int ch;
	size_t count = 0;
	
	f = fopen(name, "r");
	if(f == NULL) {
		LOG_ERR("File \"%s\" not found!\n", name);
		count = 0;
    return count;
	}
	
	while((ch = fgetc(f)) != EOF) {
		buf[count++]  = ch;
		if(count >= num) {
			break;
		}
	}
	
#if 1		// print data
	FS_DBG("read file %s:\n", name);
	for(size_t i = 0; i < count; i++) {
		FS_DBG("%c", buf[i]);
		if(i % 100 == 0) {
			osDelay(10);
		}
	}
	FS_DBG("\n");
#endif
	fclose(f);
	return count;
}

static char *get_drive (char *src, char *dst, uint32_t dst_sz) {
  uint32_t i, n;

  i = 0;
  n = 0;
  while (!n && src && src[i] && (i < dst_sz)) {
    dst[i] = src[i];

    if (dst[i] == ':') {
      n = i + 1;
    }
    i++;
  }
  if (n == dst_sz) {
    n = 0;
  }
  dst[n] = '\0';

  return (src + n);
}

static void cmd_format(char *par)
{
  char  label[12];
  char  drive[4];

  par = get_drive (par, drive, 4);
	/* Format the drive */
	if (fformat (drive, par) == fsOK) {
		LOG_DBG("Format completed.\n");
		if (fvol (drive, label, NULL) == 0) {
			if (label[0] != '\0') {
				LOG_DBG("Volume label is \"%s\"\n", label);
			}
		}
	}
	else {
		LOG_ERR("Formatting failed.\n");
	}
}
