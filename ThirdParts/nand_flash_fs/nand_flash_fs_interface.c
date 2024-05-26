#include "nand_flash_fs_interface.h"
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
static void dot_format (uint64_t val, char *sp);

void init_filesystem(uint8_t forceFormat)
{
	fsStatus stat;
  stat = finit ("N0:");
	if(stat != fsOK) {
		FS_ERR("Drive N0 initialization failed!\n");
		return;
	}
	
	stat = fmount ("N0:");
	if(forceFormat || stat == fsNoFileSystem) {
		// format
		FS_DBG("format NandFlash FS\n");
		cmd_format("N0:");
		if(fmount("N0:") != fsOK) {
			FS_ERR("mount N0: failed\n");
		}
	} else if(stat != fsOK) {
		FS_ERR("mount NandFlash err[%d]\n", stat);
	} else {
		FS_DBG("NandFlash FS ready\n");
	}
}

size_t write_file(char* name, uint8_t* buf, size_t num)
{
	FILE* f;
	
	f = fopen(name, "a");
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

/**
 *	@brief	list all the items(file & dir) info of current path. (Shown in order of creation time)
 */
void show_dir(char *mask)
{
  int64_t  free;
  uint64_t fsize;
  uint32_t files,dirs,i;
  char temp[32],ch;
  fsFileInfo info;

  FS_DBG("\n**** list [%s] ****\n", mask);
  files = 0;
  dirs  = 0;
  fsize = 0;
  info.fileID  = 0;
  while (ffind (mask,&info) == fsOK) {
    if (info.attrib & FS_FAT_ATTR_DIRECTORY) {
      i = 0;
      while (strlen((const char *)info.name+i) > 41) {
        ch = info.name[i+41];
        info.name[i+41] = 0;
        FS_DBG("\n[%-41s]", &info.name[i]);
        info.name[i+41] = ch;
        i += 41;
      }
      FS_DBG ("DIR: \"%-41s\"", &info.name[i]);
      FS_DBG ("  %04d.%02d.%02d %02d:%02d\n",
               info.time.year, info.time.mon, info.time.day,
               info.time.hr, info.time.min);
      dirs++;
    } else {
      dot_format (info.size, &temp[0]);
      i = 0;
      while (strlen((const char *)info.name+i) > 41) {
        ch = info.name[i+41];
        info.name[i+41] = 0;
        FS_DBG ("\n[%-41s]", &info.name[i]);
        info.name[i+41] = ch;
        i += 41;
      }
      FS_DBG ("%-41s %14s ", &info.name[i], temp);
      FS_DBG ("  %04d.%02d.%02d %02d:%02d\n",
               info.time.year, info.time.mon, info.time.day,
               info.time.hr, info.time.min);
      fsize += info.size;
      files++;
    }
  }
	
  if (info.fileID == 0) {
    FS_DBG ("No files...");
  }
  else {
    dot_format (fsize, &temp[0]);
    FS_DBG ("              %9d File(s)    %21s bytes", files, temp);
  }
	
  free = ffree(mask);
  if (free >= 0) {
    dot_format ((uint64_t)free, &temp[0]);
    if (dirs) {
      FS_DBG ("\n              %9d Dir(s)     %21s bytes free.\n", dirs, temp);
    }
    else {
      FS_DBG ("\n%56s bytes free.\n",temp);
    }
  }
	FS_DBG("**** list [%s] over ****\n", mask);
}

/*-----------------------------------------------------------------------------
 *        Print size in dotted format
 *----------------------------------------------------------------------------*/
static void dot_format (uint64_t val, char *sp) {

  if (val >= (uint64_t)1e12) {
    sp += sprintf (sp,"%d.",(uint32_t)(val/(uint64_t)1e12));
    val %= (uint64_t)1e12;
    sp += sprintf (sp,"%03d.",(uint32_t)(val/(uint64_t)1e9));
    val %= (uint64_t)1e9;
    sp += sprintf (sp,"%03d.",(uint32_t)(val/(uint64_t)1e6));
    val %= (uint64_t)1e6;
    sprintf (sp,"%03d.%03d",(uint32_t)(val/1000),(uint32_t)(val%1000));
    return;
  }
  if (val >= (uint64_t)1e9) {
    sp += sprintf (sp,"%d.",(uint32_t)(val/(uint64_t)1e9));
    val %= (uint64_t)1e9;
    sp += sprintf (sp,"%03d.",(uint32_t)(val/(uint64_t)1e6));
    val %= (uint64_t)1e6;
    sprintf (sp,"%03d.%03d",(uint32_t)(val/1000),(uint32_t)(val%1000));
    return;
  }
  if (val >= (uint64_t)1e6) {
    sp += sprintf (sp,"%d.",(uint32_t)(val/(uint64_t)1e6));
    val %= (uint64_t)1e6;
    sprintf (sp,"%03d.%03d",(uint32_t)(val/1000),(uint32_t)(val%1000));
    return;
  }
  if (val >= 1000) {
    sprintf (sp,"%d.%03d",(uint32_t)(val/1000),(uint32_t)(val%1000));
    return;
  }
  sprintf (sp,"%d",(uint32_t)(val));
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
