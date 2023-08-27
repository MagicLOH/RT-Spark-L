
#ifndef ___HAL_H___
#define ___HAL_H___

#include "HAL_Def.h"
#include <rtthread.h>

namespace HAL
{
    void HAL_Update();

    /* RTC */
    void RTC_Init();
    void RTC_Update();

    /* Button */
    void Button_Init();
    void Button_Update();
    uint8_t Button_GetID();
    void Button_onPressed(void *);
    void Button_onReleased(void *);
    void Button_onLongPressing(void *);

    /* SDCard */
    void SD_MountFS();
    void SD_FontLib_Update(const char *path);

    /* LCD */
    rt_err_t LCD_ShowString(rt_uint16_t x, rt_uint16_t y, rt_uint32_t size, const char *fmt, ...);


    /* External Flash (W25Q64) */
    void SPI_Flash_MountFS();
    rt_err_t SPI_Flash_Read(const char *partiton_name,
                            const char *dest_buf,
                            uint32_t start_offset,
                            uint16_t font_len);
    rt_err_t SPI_Flash_FontLibUpdate(const char *partiton_name,
                                     const char *lib_path);

    void show(uint16_t x, uint16_t y);
}


void lcd_draw_circle(rt_uint16_t x0, rt_uint16_t y0, rt_uint8_t r);
void lcd_draw_line(rt_uint16_t x1, rt_uint16_t y1, rt_uint16_t x2, rt_uint16_t y2);
void lcd_draw_rectangle(rt_uint16_t x1, rt_uint16_t y1, rt_uint16_t x2, rt_uint16_t y2);
void lcd_fill(rt_uint16_t x_start, rt_uint16_t y_start, rt_uint16_t x_end, rt_uint16_t y_end, rt_uint16_t color);

void lcd_show_num(rt_uint16_t x, rt_uint16_t y, rt_uint32_t num, rt_uint8_t len, rt_uint32_t size);
rt_err_t lcd_show_string(rt_uint16_t x, rt_uint16_t y, rt_uint32_t size, const char *fmt, ...);
rt_err_t lcd_show_image(rt_uint16_t x, rt_uint16_t y, rt_uint16_t length, rt_uint16_t wide, const rt_uint8_t *p);


#endif //_HAL_H_
