
/*
 * Auto generated Run-Time-Environment Configuration File
 *      *** Do not modify ! ***
 *
 * Project: 'nand_FS' 
 * Target:  'Target 1' 
 */

#ifndef RTE_COMPONENTS_H
#define RTE_COMPONENTS_H


/*
 * Define the Device Header File: 
 */
#define CMSIS_device_header "stm32h7xx.h"

/* ARM.FreeRTOS::RTOS:Config:CMSIS RTOS2:10.5.1 */
#define RTE_RTOS_FreeRTOS_CONFIG_RTOS2  /* RTOS FreeRTOS Config for CMSIS RTOS2 API */
/* ARM.FreeRTOS::RTOS:Core:Cortex-M:10.5.1 */
#define RTE_RTOS_FreeRTOS_CORE          /* RTOS FreeRTOS Core */
/* ARM.FreeRTOS::RTOS:Event Groups:10.5.1 */
#define RTE_RTOS_FreeRTOS_EVENTGROUPS   /* RTOS FreeRTOS Event Groups */
/* ARM.FreeRTOS::RTOS:Heap:Heap_4:10.5.1 */
#define RTE_RTOS_FreeRTOS_HEAP_4        /* RTOS FreeRTOS Heap 4 */
/* ARM.FreeRTOS::RTOS:Timers:10.5.1 */
#define RTE_RTOS_FreeRTOS_TIMERS        /* RTOS FreeRTOS Timers */
/* ARM::CMSIS:RTOS2:FreeRTOS:Cortex-M:10.5.1 */
#define RTE_CMSIS_RTOS2                 /* CMSIS-RTOS2 */
        #define RTE_CMSIS_RTOS2_FreeRTOS        /* CMSIS-RTOS2 FreeRTOS */
/* Keil.ARM Compiler::Compiler:Event Recorder:DAP:1.5.1 */
#define RTE_Compiler_EventRecorder
          #define RTE_Compiler_EventRecorder_DAP
/* Keil.ARM Compiler::Compiler:I/O:File:File System:1.2.0 */
#define RTE_Compiler_IO_File            /* Compiler I/O: File */
          #define RTE_Compiler_IO_File_FS         /* Compiler I/O: File (File System) */
/* Keil.ARM Compiler::Compiler:I/O:STDIN:Breakpoint:1.2.0 */
#define RTE_Compiler_IO_STDIN           /* Compiler I/O: STDIN */
          #define RTE_Compiler_IO_STDIN_BKPT      /* Compiler I/O: STDIN Breakpoint */
/* Keil.ARM Compiler::Compiler:I/O:STDOUT:Breakpoint:1.2.0 */
#define RTE_Compiler_IO_STDOUT          /* Compiler I/O: STDOUT */
          #define RTE_Compiler_IO_STDOUT_BKPT     /* Compiler I/O: STDOUT Breakpoint */
/* Keil.MDK-Plus::File System:CORE:LFN Debug:6.15.0 */
#define RTE_FileSystem_Core             /* File System Core */
          #define RTE_FileSystem_LFN              /* File System with Long Filename support */
          #define RTE_FileSystem_Debug            /* File System Debug Version */
/* Keil.MDK-Plus::File System:Drive:NAND:6.15.0 */
#define RTE_FileSystem_Drive_NAND_0     /* File System NAND Flash Drive 0 */

/* Keil::CMSIS Driver:NAND:Memory Bus:1.1.0 */
#define RTE_Driver_NAND_MemoryBus       /* Driver NAND Flash on Memory Bus */
/* Keil::Device:STM32Cube Framework:STM32CubeMX:2.0.0 */
#define RTE_DEVICE_FRAMEWORK_CUBE_MX
/* Keil::Device:STM32Cube HAL:Common:1.11.0 */
#define RTE_DEVICE_HAL_COMMON
/* Keil::Device:STM32Cube HAL:Cortex:1.11.0 */
#define RTE_DEVICE_HAL_CORTEX
/* Keil::Device:STM32Cube HAL:DMA:1.11.0 */
#define RTE_DEVICE_HAL_DMA
/* Keil::Device:STM32Cube HAL:GPIO:1.11.0 */
#define RTE_DEVICE_HAL_GPIO
/* Keil::Device:STM32Cube HAL:NAND:1.11.0 */
#define RTE_DEVICE_HAL_NAND
/* Keil::Device:STM32Cube HAL:PWR:1.11.0 */
#define RTE_DEVICE_HAL_PWR
/* Keil::Device:STM32Cube HAL:RCC:1.11.0 */
#define RTE_DEVICE_HAL_RCC
/* Keil::Device:STM32Cube HAL:TIM:1.11.0 */
#define RTE_DEVICE_HAL_TIM
/* Keil::Device:Startup:1.11.0 */
#define RTE_DEVICE_STARTUP_STM32H7XX    /* Device Startup for STM32H7 */


#endif /* RTE_COMPONENTS_H */
