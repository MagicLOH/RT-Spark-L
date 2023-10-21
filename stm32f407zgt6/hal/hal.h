#ifndef ___MYHAL_H___
#define ___MYHAL_H___

#include "hal_def.h"
#include "hal_config.h"
#include <rtthread.h>
#include <drv_gpio.h>

namespace hal
{
/* RTC */
void rtc_init();
void rtc_update();

/* Button */
void btn_init();
void btn_on_pressed(void *); 		// __weak
void btn_on_released(void *); 		// __weak
void btn_on_long_pressing(void *); 	// __weak
rt_uint8_t get_act_key_value();

/* SDCard */
void sd_mount_fs();
void sd_fontlib_update(const char *path);

/* LCD */
rt_err_t lcd_show_string(rt_uint16_t x, rt_uint16_t y, rt_uint32_t size,
						const char *fmt, ...);
void lcd_clean(rt_uint16_t color);

/* spi flash flash (W25Q64) */
void spi_flash_mount_fs();
rt_err_t spi_flash_read(const char *partiton_name, const char *dest_buf,
						uint32_t start_offset,uint16_t font_len);

} /* namespace hal */

void lcd_fill(rt_uint16_t x_start, rt_uint16_t y_start, rt_uint16_t x_end, rt_uint16_t y_end, rt_uint16_t color);

#endif //___MYHAL_H___
