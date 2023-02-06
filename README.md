CPU：STM32F407ZET6

LQFP144

FLASH：512KB

RAM：192KB

***

STM32CubeMX：Version 6.6.1

STM32Cube MCU Package for STM32F4 Series：Version 1.27.1

***

Toolchain：MDK-ARM Plus Version 5.33.0.0

***

使用STM32CubeMX生成工程

UART2：PA2 - TX，PA3 - RX

LED0 - PF9

LED1 - PF10

KEY0 - PE4

KEY1 - PE3

KEY2 - PE2

KEY_UP - PA0

***

* 移植RT-Thread，取已发布版本4.1.1；拷贝源码目录中`src`（内核源码），头文件目录`include`，`libcpu/arm/cortex-m4`目录（架构相关）；再从BSP中取一个工程配置相关文件`rtconfig.h`
* MDK中新建组`RT-Thread/kernal`和`RT-Thread/cortex-m4`；`kernal`中添加`src`内核源码所有的文件，`cortex-m4`中添加相应的启动文件和`cpuport.c`；Include Paths中添加头文件`include`路径，`rtconfig.h`文件路径
* 消除编译错误：包含`RT_USING_LIBC`宏；添加`sys`文件路径；`HardFault_Handler`和`PendSV_Handler`，`rt_hw_board_init`屏蔽；
* 实现时钟systick配置
* 实现控制台输出（rt_hw_console_output()）
* 实现动态内存管理（rt_system_heap_init()）

RT_USING_LIBC, USE_HAL_DRIVER, STM32F407xx, __CLK_TCK=RT_TICK_PER_SECOND, __STDC_LIMIT_MACROS, __RTTHREAD__, RT_USING_ARM_LIBC