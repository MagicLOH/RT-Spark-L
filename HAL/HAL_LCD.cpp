#include "HAL.h"
#include "HAL_Def.h"

#include <rtthread.h>
#define DGB_TAG "HAL.LCD"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include <dfs_file.h>
#include <unistd.h>

extern "C" {
#include "drv_ST7789.h"
#include "drv_ST7789_Fonts.h"
}

#define LCD_CLEAR_SEND_NUMBER 5760

extern rt_uint16_t BACK_COLOR, FORE_COLOR;


/**
 * full color on the lcd.
 *
 * @param   x_start     start of x position
 * @param   y_start     start of y position
 * @param   x_end       end of x position
 * @param   y_end       end of y position
 * @param   color       Fill color
 *
 * @return  void
 */
void lcd_fill(rt_uint16_t x_start, rt_uint16_t y_start, rt_uint16_t x_end, rt_uint16_t y_end, rt_uint16_t color)
{
	rt_uint16_t i = 0, j = 0;
	rt_uint32_t size = 0, size_remain = 0;
	rt_uint8_t *fill_buf = RT_NULL;

	size = (x_end - x_start) * (y_end - y_start) * 2;

	if (size > LCD_CLEAR_SEND_NUMBER)
	{
		/* the number of remaining to be filled */
		size_remain = size - LCD_CLEAR_SEND_NUMBER;
		size = LCD_CLEAR_SEND_NUMBER;
	}

	lcd_address_set(x_start, y_start, x_end, y_end);

	fill_buf = (rt_uint8_t *)rt_malloc(size);
	if (fill_buf)
	{
		/* fast fill */
		while (1)
		{
			for (i = 0; i < size / 2; i++)
			{
				fill_buf[2 * i] = color >> 8;
				fill_buf[2 * i + 1] = color;
			}
			lcd_write_data_buffer(fill_buf, size);

			/* Fill completed */
			if (size_remain == 0)
			{
				break;
			}

			/* calculate the number of fill next time */
			if (size_remain > LCD_CLEAR_SEND_NUMBER)
			{
				size_remain = size_remain - LCD_CLEAR_SEND_NUMBER;
			}
			else
			{
				size = size_remain;
				size_remain = 0;
			}
		}
		rt_free(fill_buf);
	}
	else
	{
		for (i = y_start; i <= y_end; i++)
		{
			for (j = x_start; j <= x_end; j++) lcd_write_half_word(color);
		}
	}
}

/**
 * display a line on the lcd.
 *
 * @param   x1      x1 position
 * @param   y1      y1 position
 * @param   x2      x2 position
 * @param   y2      y2 position
 *
 * @return  void
 */
void lcd_draw_line(rt_uint16_t x1, rt_uint16_t y1, rt_uint16_t x2, rt_uint16_t y2)
{
	rt_uint16_t t;
	rt_uint32_t i = 0;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, row, col;

	if (y1 == y2)
	{
		/* fast draw transverse line */
		lcd_address_set(x1, y1, x2, y2);

		rt_uint8_t line_buf[480] = {0};

		for (i = 0; i < x2 - x1; i++)
		{
			line_buf[2 * i] = FORE_COLOR >> 8;
			line_buf[2 * i + 1] = FORE_COLOR;
		}

		lcd_write_data_buffer(line_buf, (x2 - x1) * 2);

		return;
	}

	delta_x = x2 - x1;
	delta_y = y2 - y1;
	row = x1;
	col = y1;
	if (delta_x > 0)
	{ incx = 1; }
	else if (delta_x == 0)
	{ incx = 0; }
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
	{ incy = 1; }
	else if (delta_y == 0)
	{ incy = 0; }
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)
	{ distance = delta_x; }
	else
	{ distance = delta_y; }
	for (t = 0; t <= distance + 1; t++)
	{
		lcd_draw_point(row, col);
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			row += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			col += incy;
		}
	}
}

/**
 * display a rectangle on the lcd.
 *
 * @param   x1      x1 position
 * @param   y1      y1 position
 * @param   x2      x2 position
 * @param   y2      y2 position
 *
 * @return  void
 */
void lcd_draw_rectangle(rt_uint16_t x1, rt_uint16_t y1, rt_uint16_t x2, rt_uint16_t y2)
{
	lcd_draw_line(x1, y1, x2, y1);
	lcd_draw_line(x1, y1, x1, y2);
	lcd_draw_line(x1, y2, x2, y2);
	lcd_draw_line(x2, y1, x2, y2);
}

/**
 * display a circle on the lcd.
 *
 * @param   x       x position of Center
 * @param   y       y position of Center
 * @param   r       radius
 *
 * @return  void
 */
void lcd_draw_circle(rt_uint16_t x0, rt_uint16_t y0, rt_uint8_t r)
{
	int a, b;
	int di;
	a = 0;
	b = r;
	di = 3 - (r << 1);
	while (a <= b)
	{
		lcd_draw_point(x0 - b, y0 - a);
		lcd_draw_point(x0 + b, y0 - a);
		lcd_draw_point(x0 - a, y0 + b);
		lcd_draw_point(x0 - b, y0 - a);
		lcd_draw_point(x0 - a, y0 - b);
		lcd_draw_point(x0 + b, y0 + a);
		lcd_draw_point(x0 + a, y0 - b);
		lcd_draw_point(x0 + a, y0 + b);
		lcd_draw_point(x0 - b, y0 + a);
		a++;
		//Bresenham
		if (di < 0)
		{ di += 4 * a + 6; }
		else
		{
			di += 10 + 4 * (a - b);
			b--;
		}
		lcd_draw_point(x0 + a, y0 + b);
	}
}

static void lcd_show_char(rt_uint16_t x, rt_uint16_t y, rt_uint8_t data, rt_uint32_t size)
{
	rt_uint8_t temp;
	rt_uint8_t num = 0;;
	rt_uint8_t pos, t;
	rt_uint16_t colortemp = FORE_COLOR;
	rt_uint8_t *font_buf = RT_NULL;

	if (x > LCD_W - size / 2 || y > LCD_H - size)return;

	data = data - ' ';
#ifdef ASC2_1608
	if (size == 16)
	{
		lcd_address_set(x, y, x + size / 2 - 1, y + size - 1);//(x,y,x+8-1,y+16-1)

		font_buf = (rt_uint8_t *)rt_malloc(size * size);
		if (!font_buf)
		{
			/* fast show char */
			for (pos = 0; pos < size * (size / 2) / 8; pos++)
			{
				temp = asc2_1608[(rt_uint16_t)data * size * (size / 2) / 8 + pos];
				for (t = 0; t < 8; t++)
				{
					if (temp & 0x80)
					{ colortemp = FORE_COLOR; }
					else
					{ colortemp = BACK_COLOR; }
					lcd_write_half_word(colortemp);
					temp <<= 1;
				}
			}
		}
		else
		{
			for (pos = 0; pos < size * (size / 2) / 8; pos++)
			{
				temp = asc2_1608[(rt_uint16_t)data * size * (size / 2) / 8 + pos];
				for (t = 0; t < 8; t++)
				{
					if (temp & 0x80)
					{ colortemp = FORE_COLOR; }
					else
					{ colortemp = BACK_COLOR; }
					font_buf[2 * (8 * pos + t)] = colortemp >> 8;
					font_buf[2 * (8 * pos + t) + 1] = colortemp;
					temp <<= 1;
				}
			}
			lcd_write_data_buffer(font_buf, size * size);
			rt_free(font_buf);
		}
	}
	else
#endif

#ifdef ASC2_2412
	if (size == 24)
	{
		lcd_address_set(x, y, x + size / 2 - 1, y + size - 1);

		font_buf = (rt_uint8_t *)rt_malloc(size * size);
		if (!font_buf)
		{
			/* fast show char */
			for (pos = 0; pos < (size * 16) / 8; pos++)
			{
				temp = asc2_2412[(rt_uint16_t)data * (size * 16) / 8 + pos];
				if (pos % 2 == 0)
				{
					num = 8;
				}
				else
				{
					num = 4;
				}

				for (t = 0; t < num; t++)
				{
					if (temp & 0x80)
					{ colortemp = FORE_COLOR; }
					else
					{ colortemp = BACK_COLOR; }
					lcd_write_half_word(colortemp);
					temp <<= 1;
				}
			}
		}
		else
		{
			for (pos = 0; pos < (size * 16) / 8; pos++)
			{
				temp = asc2_2412[(rt_uint16_t)data * (size * 16) / 8 + pos];
				if (pos % 2 == 0)
				{
					num = 8;
				}
				else
				{
					num = 4;
				}

				for (t = 0; t < num; t++)
				{
					if (temp & 0x80)
					{ colortemp = FORE_COLOR; }
					else
					{ colortemp = BACK_COLOR; }
					if (num == 8)
					{
						font_buf[2 * (12 * (pos / 2) + t)] = colortemp >> 8;
						font_buf[2 * (12 * (pos / 2) + t) + 1] = colortemp;
					}
					else
					{
						font_buf[2 * (8 + 12 * (pos / 2) + t)] = colortemp >> 8;
						font_buf[2 * (8 + 12 * (pos / 2) + t) + 1] = colortemp;
					}
					temp <<= 1;
				}
			}
			lcd_write_data_buffer(font_buf, size * size);
			rt_free(font_buf);
		}
	}
	else
#endif

#ifdef ASC2_3216
	if (size == 32)
	{
		lcd_address_set(x, y, x + size / 2 - 1, y + size - 1);

		font_buf = (rt_uint8_t *)rt_malloc(size * size);
		if (!font_buf)
		{
			/* fast show char */
			for (pos = 0; pos < size * (size / 2) / 8; pos++)
			{
				temp = asc2_3216[(rt_uint16_t)data * size * (size / 2) / 8 + pos];
				for (t = 0; t < 8; t++)
				{
					if (temp & 0x80)
					{ colortemp = FORE_COLOR; }
					else
					{ colortemp = BACK_COLOR; }
					lcd_write_half_word(colortemp);
					temp <<= 1;
				}
			}
		}
		else
		{
			for (pos = 0; pos < size * (size / 2) / 8; pos++)
			{
				temp = asc2_3216[(rt_uint16_t)data * size * (size / 2) / 8 + pos];
				for (t = 0; t < 8; t++)
				{
					if (temp & 0x80)
					{ colortemp = FORE_COLOR; }
					else
					{ colortemp = BACK_COLOR; }
					font_buf[2 * (8 * pos + t)] = colortemp >> 8;
					font_buf[2 * (8 * pos + t) + 1] = colortemp;
					temp <<= 1;
				}
			}
			lcd_write_data_buffer(font_buf, size * size);
			rt_free(font_buf);
		}
	}
	else
#endif
	{
		LOG_E("There is no any define ASC2_1208 && ASC2_2412 && ASC2_2416 && ASC2_3216 !");
	}
}

/**
 * @brief just show chinese font on lcd
 * @param x
 * @param y
 * @param size
 * @param pstr
 */
static rt_err_t lcd_show_chinese(rt_uint16_t x, rt_uint16_t y,
								 const char *pdata, const rt_uint8_t fsize)
{
	/* Calculate chinese font's direction */
	uint8_t byte_High = *pdata;
	uint8_t byte_Low = *(pdata + 1);
	if (byte_Low < 0x7F)
	{
		byte_Low = byte_Low - 0x40;
	}
	else
	{
		byte_Low = byte_Low - 0x41;
	}
	byte_High = byte_High - 0x81;
	uint32_t font_used = fsize * fsize / 8; // gbk 单字所用字节数
	uint32_t font_addr = (190 * byte_High + byte_Low) * font_used; // 计算出汉字所在字库位置
//    LOG_D("font_addr = 0x%X", font_addr);

	rt_err_t err = RT_EOK;
	char buf[128] = {0};
	/* offset indicate direction to read corresponding font Hex data */
	err = HAL::SPI_Flash_Read("font", buf, font_addr, font_used);
	if (err != RT_EOK)
	{
		LOG_E("found lib in flash falied!");
		return -RT_ERROR;
	}

	/* Draw a chinese font */
	uint16_t x0 = x;
	for (int j = 0; j < font_used; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			if (buf[j] >> i & 0x01)
			{
				lcd_draw_point(x, y);
			}
			x++;
		}
		if (x - x0 == fsize)
		{
			y++;
			x = x0;
		}
	}

	return RT_EOK;
}

/**
 * Clean screen
 */
void HAL::LCD_Clean(rt_uint16_t color)
{
	lcd_fill(0, 0, LCD_W, LCD_H, color);
}

/**
 * display the string on the lcd.
 *
 * @param   x       x position
 * @param   y       y position
 * @param   size    size of font
 * @param   p       the string to be display
 *
 * @return   0: display success
 *          -1: size of font is not support
 */
rt_err_t HAL::LCD_ShowString(rt_uint16_t x, rt_uint16_t y,
							 rt_uint32_t size, const char *fmt, ...)
{
	va_list args;
	rt_uint8_t buf[MAX_CHARS_DISP + 1] = {0};
	rt_uint8_t *p = RT_NULL;

	if (size != 16 && size != 24 && size != 32)
	{
		LOG_E("font size(%d) is not support!", size);
		return -RT_ERROR;
	}

	va_start(args, fmt);
	rt_vsnprintf((char *)buf, sizeof(buf), (const char *)fmt, args);
	va_end(args);

	p = buf;
	while (*p != '\0')
	{
		if (x > LCD_W - size / 2)
		{
			x = 0;
			y +=
				size;
		}
		if (y > LCD_H - size)
		{
			y = x = 0;
			lcd_clear(RED);
		}

		if ((*p == '\r' && *(p + 1) == '\n')) // windows(CR+LF)
		{
			p += 2;
			y +=
				size;
			x = 0;
			continue;
		}
		else if (*p == '\n' || *p == '\r') // unix(LF) or mac(CR)
		{
			p++;
			y +=
				size;
			x = 0;
			continue;
		}

		if (*p < 0x80) // ASCII
		{
			if (*p == ' ') // skip space
			{
				++
					p;
				x += size / 2;
				continue;
			}

			lcd_show_char(x, y, *p, size
			);
			x += size / 2;
			++
				p;
		}
		else // Chinese
		{
			if (x + size > LCD_W)
			{
				x = 0;
				y +=
					size;
//				p+=2;
				continue;
			}
			lcd_show_chinese(x, y,
							 (char *)p, size);
			x +=
				size;
			p += 2; // chinese font need extra offset
		}

	}
//	if (*p < 0x80) /*!< Display Ascii character*/
//	{
//		while (*p != '\0')
//		{
//			if (x > LCD_W - size / 2)
//			{
//				x = 0;
//				y += size;
//			}
//			if (y > LCD_H - size)
//			{
//				y = x = 0;
//				lcd_clear(RED);
//			}
//			lcd_show_char(x, y, *p, size);
//			x += size / 2;
//			p++;
//		}
//	}
//	else /*!< Display Chinese character*/
//	{
//		while (*p)
//		{
//			if (x > LCD_W - size / 2)
//			{
//				x = 0;
//				y += size;
//			}
//			if (y > LCD_H - size)
//			{
//				y = x = 0;
//				lcd_clear(RED); // display false color
//			}
//
//			if (*p == ' ') // display next character
//			{
//				p++;
//				x += size / 2;
//				continue;
//			}
//			else if ((*p == '\r' && *(p + 1) == '\n')) // unix(LF)
//			{
//				p += 2;
//				y += size;
//				x = 0;
//				continue;
//			}
//			else if (*p == '\n') // or windows(CR+LF)
//			{
//				p++;
//				y += size;
//				x = 0;
//				continue;
//			}
//
//			lcd_show_chinese(x, y, (char *)p, (LCD_FontSize_t)size);
//			x += size;
//			p += 2; // chinese font need extra offset
//		}
//	}

	return RT_EOK;
}

#if defined(LCD_TEST)
void HAL::show(uint16_t x, uint16_t y)
{
	const char zhong[] = {
		0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xFC, 0x1F, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10,
		0x84, 0x10, 0x84, 0x10, 0xFC, 0x1F, 0x84, 0x10, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00,
	};/*"中",0*/

	uint16_t x0 = x;
	for (int j = 0; j < 16 * 16 / 8; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			if (zhong[j] >> i & 0x01)
			{
				lcd_draw_point(x, y);
			}
			x++;
		}
		if (x - x0 == 16)
		{
			y++;
			x = x0;
		}
	}
}

void print_one_byte(uint16_t font_used)
{
	char buf[128] = {0};
//    for (int i = 0; i < font_used; i++)
//    {
//        rt_kprintf("0x%02X ", buf[i]);
//    }
//    rt_kprintf("\n");
}
#endif

/**
 * display the number on the lcd.
 *
 * @param   x       x position
 * @param   y       y position
 * @param   num     number
 * @param   len     length of number
 * @param   size    size of font
 *
 * @return  void
 */
void lcd_show_num(rt_uint16_t x, rt_uint16_t y, rt_uint32_t num, rt_uint8_t len, rt_uint32_t size)
{
//	lcd_show_string(x, y, size, "%d", num);
}

/**
 * display the image on the lcd.
 *
 * @param   x       x position
 * @param   y       y position
 * @param   length  length of image
 * @param   wide    wide of image
 * @param   p       image
 *
 * @return   0: display success
 *          -1: the image is too large
 */
rt_err_t lcd_show_image(rt_uint16_t x, rt_uint16_t y, rt_uint16_t length, rt_uint16_t wide, const rt_uint8_t *p)
{
	RT_ASSERT(p);

	if (x + length > LCD_W || y + wide > LCD_H)
	{
		return -RT_ERROR;
	}

	lcd_address_set(x, y, x + length - 1, y + wide - 1);

	lcd_write_data_buffer(p, length * wide * 2);

	return RT_EOK;
}


