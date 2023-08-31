#ifndef __READER_MODEL_H
#define __READER_MODEL_H

extern "C"
{
#include "drv_st7789.h"
}

#include <rtthread.h>
#include "HAL.h"

#ifdef PKG_USING_LVGL
#include <lvgl.h>
#include "Account.h"
#endif

const int MAX_TITLE_LEN = 32;
const int MAX_FONTS_DISP = LCD_W * (LCD_H - CHN_FONT_24x24) / CHN_FONT_24x24 * 2;

namespace Page
{
	class ReaderModel
	{
	public:
		struct NovelInfo
		{
			const char *Name;       // novel name
			const char *Path;       // novel file path
			uint32_t FileSize;      // novel file size
			uint32_t TotalFonts;    // novel total font count
			uint32_t TotalPages;    // total novel pages (assemble)
			uint32_t RealPages;     // calculate result
		};

		/* Page List */
		struct PageList
		{
			uint8_t status;

			uint16_t FontCount;                 // current total font number in one page
			char Context[MAX_FONTS_DISP];       // MAX contain chinese font
		};

	public:
		ReaderModel();
		~ReaderModel();

	public:
		void Init();
		void Deinit();

		uint32_t getFileSize(int fd);
		uint16_t getAPageFontCount();
		PageList *getPage(uint32_t index);

		bool loadNovel(const char *path, const char *name);
		void toNextPage();
		void toPrevPage();

		bool isLoaded(const char *name) const; // judge is load by novel's name
		bool isBusy();

	public:
		PageList *_RootList;  // root list for whole novel
		uint32_t m_nOldIndex;
		uint32_t m_nCurrentIndex;
		NovelInfo *m_novel_info;

	private:
		Account *account;
	};
}

#endif
