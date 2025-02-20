CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR cortex-m4)
#SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(CMAKE_C_COMPILER "E:/_Tools/env-windows-v1.3.5/tools/ConEmu/ConEmu/../../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-gcc.exe")
SET(CMAKE_ASM_COMPILER "E:/_Tools/env-windows-v1.3.5/tools/ConEmu/ConEmu/../../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-gcc.exe")
SET(CMAKE_C_FLAGS " -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g")
SET(CMAKE_ASM_FLAGS " -c -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -x assembler-with-cpp -Wa,-mimplicit-it=thumb  -gdwarf-2")
SET(CMAKE_C_COMPILER_WORKS TRUE)

SET(CMAKE_CXX_COMPILER "E:/_Tools/env-windows-v1.3.5/tools/ConEmu/ConEmu/../../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-g++.exe")
SET(CMAKE_CXX_FLAGS " -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g")
SET(CMAKE_CXX_COMPILER_WORKS TRUE)

SET(CMAKE_OBJCOPY "E:/_Tools/env-windows-v1.3.5/tools/ConEmu/ConEmu/../../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-objcopy.exe")
SET(CMAKE_SIZE "E:/_Tools/env-windows-v1.3.5/tools/ConEmu/ConEmu/../../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-size.exe")

SET(CMAKE_EXE_LINKER_FLAGS " -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Wl,--gc-sections,-Map=rt-thread.map,-cref,-u,Reset_Handler -T ${CMAKE_SOURCE_DIR}/board/linker_scripts/link.lds")

SET(CMAKE_CXX_STANDARD 14)
PROJECT(rtthread C CXX ASM)
INCLUDE_DIRECTORIES(
	applications
	board/board_drivers
	board/board_drivers/lcd_st7789
	board/board_drivers/spi_falsh_w25q64
	../rt-thread/components/libc/compilers/common/include
	../rt-thread/components/libc/compilers/newlib
	../rt-thread/components/libc/cplusplus
	../rt-thread/libcpu/arm/common
	../rt-thread/libcpu/arm/cortex-m4
	../rt-thread/components/drivers/include
	../rt-thread/components/drivers/spi
	../rt-thread/components/drivers/spi/sfud/inc
	board
	board/CubeMX_Config/Inc
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/libraries/HAL_Drivers",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/libraries/HAL_Drivers/config",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/libraries/HAL_Drivers/CMSIS/Include",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/libraries/HAL_Drivers/drv_flash",
	../rt-thread/components/fal/inc
	../rt-thread/components/dfs/include
	../rt-thread/components/dfs/filesystems/elmfat
	../rt-thread/components/dfs/filesystems/romfs
	../rt-thread/components/finsh
	hal
	.
	../rt-thread/include
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Inc",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/libraries/STM32F4xx_HAL/CMSIS/Device/ST/STM32F4xx/Include",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/core",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/draw",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/font",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/hal",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/misc",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/widgets",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/draw/arm2d",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/draw/sdl",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/draw/stm32_dma2d",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/draw/sw",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/draw/swm341_dma2d",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/draw/nxp/pxp",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/draw/nxp/vglite",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/layouts",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/libs",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/others",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/themes",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/layouts/flex",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/layouts/grid",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/libs/bmp",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/libs/ffmpeg",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/libs/freetype",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/libs/fsdrv",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/libs/gif",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/libs/png",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/libs/qrcode",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/libs/rlottie",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/libs/sjpg",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/others/fragment",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/others/gridnav",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/others/ime",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/others/imgfont",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/others/monkey",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/others/msg",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/others/snapshot",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/themes/basic",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/themes/default",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/themes/mono",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/animimg",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/calendar",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/chart",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/colorwheel",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/imgbtn",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/keyboard",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/led",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/list",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/menu",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/meter",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/msgbox",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/span",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/spinbox",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/spinner",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/tabview",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/tileview",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/src/extra/widgets/win",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/demos",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/demos/benchmark",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/demos/keypad_encoder",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/demos/music",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/demos/stress",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/demos/widgets",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/demos/music/assets",
	board/ports/lvgl
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/LVGL-v8.3.5/env_support/rt-thread",
	"E:/ProjectsDir/STM32F407ZGT6/rt-spark1_test/offline_pkgs/MultiButton",
	board/ports
	board/ports/fal
	../rt-thread/components/libc/posix/io/poll
	../rt-thread/components/libc/posix/io/stdio
	../rt-thread/components/libc/posix/ipc
	applications/utils/Reader-beta
)

ADD_DEFINITIONS(
	-DHAVE_CCONFIG_H
	-DRT_USING_LIBC
	-DRT_USING_NEWLIB
	-DSTM32F407xx
	-DUSE_HAL_DRIVER
	-D_POSIX_C_SOURCE=1
	-D__RTTHREAD__
)

SET(PROJECT_SOURCES
	applications/app.cpp
	applications/main.cpp
	board/board_drivers/spi_falsh_w25q64/drv_spi_falsh.c
	board/board_drivers/lcd_st7789/drv_st7789.c
	board/board_drivers/spi_falsh_w25q64/drv_soft_spi_flash.c
	board/board_drivers/drv_filesystem.c
	../rt-thread/components/libc/compilers/common/cctype.c
	../rt-thread/components/libc/compilers/common/cstdio.c
	../rt-thread/components/libc/compilers/common/cstdlib.c
	../rt-thread/components/libc/compilers/common/cstring.c
	../rt-thread/components/libc/compilers/common/ctime.c
	../rt-thread/components/libc/compilers/common/cwchar.c
	../rt-thread/components/libc/compilers/newlib/syscalls.c
	../rt-thread/components/libc/cplusplus/cxx_crt_init.c
	../rt-thread/components/libc/cplusplus/cxx_crt.cpp
	../rt-thread/components/libc/cplusplus/cxx_Thread.cpp
	../rt-thread/components/libc/cplusplus/cxx_Semaphore.cpp
	../rt-thread/components/libc/cplusplus/cxx_Mutex.cpp
	../rt-thread/libcpu/arm/common/backtrace.c
	../rt-thread/libcpu/arm/common/div0.c
	../rt-thread/libcpu/arm/common/showmem.c
	../rt-thread/libcpu/arm/cortex-m4/context_gcc.S
	../rt-thread/libcpu/arm/cortex-m4/cpuport.c
	../rt-thread/components/drivers/ipc/completion.c
	../rt-thread/components/drivers/ipc/dataqueue.c
	../rt-thread/components/drivers/ipc/pipe.c
	../rt-thread/components/drivers/ipc/ringblk_buf.c
	../rt-thread/components/drivers/ipc/ringbuffer.c
	../rt-thread/components/drivers/ipc/waitqueue.c
	../rt-thread/components/drivers/ipc/workqueue.c
	../rt-thread/components/drivers/misc/pin.c
	../rt-thread/components/drivers/misc/rt_drv_pwm.c
	../rt-thread/components/drivers/rtc/rtc.c
	../rt-thread/components/drivers/sdio/block_dev.c
	../rt-thread/components/drivers/sdio/mmc.c
	../rt-thread/components/drivers/sdio/mmcsd_core.c
	../rt-thread/components/drivers/sdio/sd.c
	../rt-thread/components/drivers/sdio/sdio.c
	../rt-thread/components/drivers/serial/serial.c
	../rt-thread/components/drivers/spi/sfud/src/sfud.c
	../rt-thread/components/drivers/spi/sfud/src/sfud_sfdp.c
	../rt-thread/components/drivers/spi/spi_core.c
	../rt-thread/components/drivers/spi/spi_dev.c
	../rt-thread/components/drivers/spi/spi_flash_sfud.c
	../libraries/HAL_Drivers/drv_common.c
	../libraries/HAL_Drivers/drv_flash/drv_flash_f4.c
	../libraries/HAL_Drivers/drv_gpio.c
	../libraries/HAL_Drivers/drv_pwm.c
	../libraries/HAL_Drivers/drv_sdio.c
	../libraries/HAL_Drivers/drv_spi.c
	../libraries/HAL_Drivers/drv_usart.c
	../libraries/STM32F4xx_HAL/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.s
	board/CubeMX_Config/Src/stm32f4xx_hal_msp.c
	board/board.c
	../rt-thread/components/fal/src/fal.c
	../rt-thread/components/fal/src/fal_partition.c
	../rt-thread/components/fal/src/fal_flash.c
	../rt-thread/components/fal/samples/porting/fal_flash_sfud_port.c
	../rt-thread/components/fal/src/fal_rtt.c
	../rt-thread/components/dfs/filesystems/elmfat/dfs_elm.c
	../rt-thread/components/dfs/filesystems/elmfat/ff.c
	../rt-thread/components/dfs/filesystems/elmfat/ffunicode.c
	../rt-thread/components/dfs/filesystems/romfs/dfs_romfs.c
	../rt-thread/components/dfs/filesystems/romfs/romfs.c
	../rt-thread/components/dfs/src/dfs.c
	../rt-thread/components/dfs/src/dfs_file.c
	../rt-thread/components/dfs/src/dfs_fs.c
	../rt-thread/components/dfs/src/dfs_posix.c
	../rt-thread/components/finsh/shell.c
	../rt-thread/components/finsh/msh.c
	../rt-thread/components/finsh/msh_parse.c
	../rt-thread/components/finsh/cmd.c
	../rt-thread/components/finsh/msh_file.c
	hal/hal_sdcard.cpp
	hal/hal_button.cpp
	hal/hal.cpp
	hal/hal_spi_flash.cpp
	hal/hal_lcd.cpp
	../rt-thread/src/clock.c
	../rt-thread/src/components.c
	../rt-thread/src/device.c
	../rt-thread/src/idle.c
	../rt-thread/src/ipc.c
	../rt-thread/src/irq.c
	../rt-thread/src/kservice.c
	../rt-thread/src/mem.c
	../rt-thread/src/mempool.c
	../rt-thread/src/object.c
	../rt-thread/src/scheduler.c
	../rt-thread/src/thread.c
	../rt-thread/src/timer.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_lptim.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_qspi.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp_ex.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_usart.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fsmc.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sram.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cec.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_sdmmc.c
	../libraries/STM32F4xx_HAL/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
	../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmc.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/others/imgfont/lv_imgfont.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/themes/mono/lv_theme_mono.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_34.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_30.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sw/lv_draw_sw_img.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_cover_2_large.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/themes/default/lv_theme_default.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_cover_2.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/lv_demo_music_main.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_math.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_color.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/libs/png/lv_png.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sw/lv_draw_sw_gradient.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_list_play_large.c
	../offline_pkgs/LVGL-v8.3.5/src/core/lv_obj_scroll.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/lv_img_cache.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/win/lv_win.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_38.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_36.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sdl/lv_draw_sdl_texture_cache.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sw/lv_draw_sw_letter.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_corner_right_large.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/libs/qrcode/qrcodegen.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_corner_right.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/lv_img_decoder.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/lv_draw_transform.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/imgbtn/lv_imgbtn.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_mem.c
	../offline_pkgs/LVGL-v8.3.5/src/widgets/lv_arc.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sw/lv_draw_sw.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sdl/lv_draw_sdl_polygon.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_logo.c
	../offline_pkgs/LVGL-v8.3.5/src/core/lv_theme.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font.c
	../offline_pkgs/LVGL-v8.3.5/demos/stress/lv_demo_stress.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_46.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_txt_ap.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sdl/lv_draw_sdl_img.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sw/lv_draw_sw_line.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_cover_3_large.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/chart/lv_chart.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_icon_3_large.c
	../offline_pkgs/LVGL-v8.3.5/src/core/lv_obj_draw.c
	../offline_pkgs/LVGL-v8.3.5/src/widgets/lv_btn.c
	../offline_pkgs/LVGL-v8.3.5/src/widgets/lv_bar.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sdl/lv_draw_sdl_mask.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/libs/freetype/lv_freetype.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_wave_top.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/lv_draw_line.c
	../offline_pkgs/LVGL-v8.3.5/src/core/lv_event.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_lru.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_simsun_16_cjk.c
	../offline_pkgs/LVGL-v8.3.5/src/widgets/lv_checkbox.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/calendar/lv_calendar.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/swm341_dma2d/lv_gpu_swm341_dma2d.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_cover_1_large.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/others/fragment/lv_fragment_manager.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_icon_4.c
	../offline_pkgs/LVGL-v8.3.5/src/core/lv_group.c
	../offline_pkgs/LVGL-v8.3.5/src/widgets/lv_canvas.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_tlsf.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sdl/lv_draw_sdl_stack_blur.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_prev_large.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/libs/fsdrv/lv_fs_fatfs.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_next_large.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_18.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/lv_draw_rect.c
	../offline_pkgs/LVGL-v8.3.5/demos/widgets/lv_demo_widgets.c
	../offline_pkgs/LVGL-v8.3.5/src/widgets/lv_dropdown.c
	../offline_pkgs/LVGL-v8.3.5/src/widgets/lv_img.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/animimg/lv_animimg.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_slider_knob.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/nxp/vglite/lv_vglite_utils.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_icon_1.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/others/msg/lv_msg.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_wave_bottom.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/libs/sjpg/tjpgd.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/spinner/lv_spinner.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/led/lv_led.c
	../offline_pkgs/LVGL-v8.3.5/src/widgets/lv_slider.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_utils.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/nxp/pxp/lv_gpu_nxp_pxp.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sdl/lv_draw_sdl_utils.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_cover_3.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/libs/fsdrv/lv_fs_posix.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_cover_1.c
	../offline_pkgs/LVGL-v8.3.5/src/core/lv_indev_scroll.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_fmt_txt.c
	../offline_pkgs/LVGL-v8.3.5/src/widgets/lv_line.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_40.c
	../offline_pkgs/LVGL-v8.3.5/src/widgets/lv_objx_templ.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sw/lv_draw_sw_blend.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_loop_large.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/libs/ffmpeg/lv_ffmpeg.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_list_border_large.c
	../offline_pkgs/LVGL-v8.3.5/src/core/lv_indev.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_wave_top_large.c
	../offline_pkgs/LVGL-v8.3.5/src/core/lv_refr.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/lv_extra.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_42.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sdl/lv_draw_sdl_layer.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sw/lv_draw_sw_polygon.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_icon_4_large.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/libs/fsdrv/lv_fs_stdio.c
	../offline_pkgs/LVGL-v8.3.5/demos/benchmark/assets/lv_font_bechmark_montserrat_12_compr_az.c.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/lv_draw_layer.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_loader.c
	../offline_pkgs/LVGL-v8.3.5/demos/benchmark/assets/img_benchmark_cogwheel_argb.c
	../offline_pkgs/LVGL-v8.3.5/src/widgets/lv_roller.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_area.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/others/snapshot/lv_snapshot.c
	../offline_pkgs/LVGL-v8.3.5/src/widgets/lv_switch.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/nxp/vglite/lv_draw_vglite_rect.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sw/lv_draw_sw_layer.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_wave_bottom_large.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_14.c
	../offline_pkgs/LVGL-v8.3.5/demos/widgets/assets/img_clothes.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/colorwheel/lv_colorwheel.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_16.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_unscii_16.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_fs.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/layouts/flex/lv_flex.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sw/lv_draw_sw_arc.c
	../offline_pkgs/LVGL-v8.3.5/demos/benchmark/assets/img_benchmark_cogwheel_rgb565a8.c
	../offline_pkgs/LVGL-v8.3.5/src/core/lv_disp.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_10.c
	../offline_pkgs/LVGL-v8.3.5/demos/benchmark/assets/img_benchmark_cogwheel_chroma_keyed.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/libs/sjpg/lv_sjpg.c
	../offline_pkgs/LVGL-v8.3.5/src/widgets/lv_label.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_bidi.c
	../offline_pkgs/LVGL-v8.3.5/src/widgets/lv_table.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/span/lv_span.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/libs/png/lodepng.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_list_pause.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sw/lv_draw_sw_transform.c
	../offline_pkgs/LVGL-v8.3.5/demos/benchmark/assets/img_benchmark_cogwheel_rgb.c
	../offline_pkgs/LVGL-v8.3.5/src/core/lv_obj_tree.c
	../offline_pkgs/LVGL-v8.3.5/demos/widgets/assets/img_lvgl_logo.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/lv_draw_label.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_20.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/tileview/lv_tileview.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sdl/lv_draw_sdl.c
	../offline_pkgs/LVGL-v8.3.5/src/hal/lv_hal_tick.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sw/lv_draw_sw_dither.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_next.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/libs/gif/gifdec.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_corner_left.c
	../offline_pkgs/LVGL-v8.3.5/demos/benchmark/lv_demo_benchmark.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_12.c
	../offline_pkgs/LVGL-v8.3.5/demos/benchmark/assets/img_benchmark_cogwheel_indexed16.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_unscii_8.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_ll.c
	../offline_pkgs/LVGL-v8.3.5/src/widgets/lv_textarea.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_icon_2.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/nxp/pxp/lv_gpu_nxp_pxp_osa.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_rnd.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/msgbox/lv_msgbox.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/lv_draw_mask.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_22.c
	../offline_pkgs/LVGL-v8.3.5/src/widgets/lv_btnmatrix.c
	../offline_pkgs/LVGL-v8.3.5/src/hal/lv_hal_indev.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/nxp/pxp/lv_draw_pxp_blend.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/nxp/pxp/lv_draw_pxp.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_corner_left_large.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/libs/fsdrv/lv_fs_win32.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_list_border.c
	../offline_pkgs/LVGL-v8.3.5/src/core/lv_obj_style.c
	../offline_pkgs/LVGL-v8.3.5/src/core/lv_obj_class.c
	../offline_pkgs/LVGL-v8.3.5/demos/benchmark/assets/lv_font_bechmark_montserrat_28_compr_az.c.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_async.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_style.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/arm2d/lv_gpu_arm2d.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/libs/bmp/lv_bmp.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_play_large.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sdl/lv_draw_sdl_label.c
	../offline_pkgs/LVGL-v8.3.5/demos/benchmark/assets/lv_font_bechmark_montserrat_16_compr_az.c.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/lv_draw_arc.c
	../offline_pkgs/LVGL-v8.3.5/demos/keypad_encoder/lv_demo_keypad_encoder.c
	../offline_pkgs/LVGL-v8.3.5/src/core/lv_obj_style_gen.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_24.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_printf.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_log.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sw/lv_draw_sw_rect.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_slider_knob_large.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/keyboard/lv_keyboard.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_list_play.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/others/ime/lv_ime_pinyin.c
	../offline_pkgs/LVGL-v8.3.5/src/core/lv_obj_pos.c
	../offline_pkgs/LVGL-v8.3.5/demos/benchmark/assets/img_benchmark_cogwheel_alpha16.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_32.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_style_gen.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/nxp/vglite/lv_vglite_buf.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_icon_3.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sdl/lv_draw_sdl_line.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_list_pause_large.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_dejavu_16_persian_hebrew.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/others/fragment/lv_fragment.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_26.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/libs/rlottie/lv_rlottie.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/lv_demo_music_list.c
	../offline_pkgs/LVGL-v8.3.5/src/hal/lv_hal_disp.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_44.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/nxp/vglite/lv_draw_vglite.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/libs/gif/lv_gif.c
	../offline_pkgs/LVGL-v8.3.5/demos/widgets/assets/img_demo_widgets_avatar.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/themes/basic/lv_theme_basic.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_pause.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/menu/lv_menu.c
	../offline_pkgs/LVGL-v8.3.5/src/core/lv_obj.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/list/lv_list.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/tabview/lv_tabview.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_48.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_templ.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_loop.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sdl/lv_draw_sdl_composite.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_rnd_large.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/calendar/lv_calendar_header_dropdown.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_28.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/others/gridnav/lv_gridnav.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_txt.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/spinbox/lv_spinbox.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/nxp/vglite/lv_draw_vglite_arc.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/layouts/grid/lv_grid.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_play.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/libs/qrcode/lv_qrcode.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_corner_large.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/lv_draw.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_12_subpx.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_anim_timeline.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_timer.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sdl/lv_draw_sdl_arc.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/calendar/lv_calendar_header_arrow.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sdl/lv_draw_sdl_rect.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_icon_2_large.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/lv_draw_img.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/lv_draw_triangle.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_28_compressed.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_anim.c
	../offline_pkgs/LVGL-v8.3.5/src/font/lv_font_montserrat_8.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/nxp/vglite/lv_draw_vglite_blend.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_prev.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_btn_pause_large.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/lv_img_buf.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/lv_demo_music.c
	../offline_pkgs/LVGL-v8.3.5/src/misc/lv_gc.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/others/monkey/lv_monkey.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/sdl/lv_draw_sdl_bg.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/nxp/vglite/lv_draw_vglite_line.c
	../offline_pkgs/LVGL-v8.3.5/src/draw/stm32_dma2d/lv_gpu_stm32_dma2d.c
	../offline_pkgs/LVGL-v8.3.5/demos/music/assets/img_lv_demo_music_icon_1_large.c
	../offline_pkgs/LVGL-v8.3.5/src/extra/widgets/meter/lv_meter.c
	board/ports/lvgl/lv_port.cpp
	board/ports/lvgl/lv_port_disp.cpp
	board/ports/lvgl/lv_port_fs.cpp
	board/ports/lvgl/lv_port_indev.cpp
	../offline_pkgs/LVGL-v8.3.5/env_support/rt-thread/lv_rt_thread_port.cpp
	../offline_pkgs/MultiButton/multi_button.c
	board/ports/fal/fal_spi_flash_sfud_port.c
	applications/utils/Reader-beta/Reader.cpp
)

LINK_DIRECTORIES(
)

LINK_LIBRARIES(
	c
	m
)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf ${PROJECT_SOURCES})
ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME}.elf POST_BUILD 
COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin COMMAND ${CMAKE_SIZE} ${CMAKE_PROJECT_NAME}.elf)