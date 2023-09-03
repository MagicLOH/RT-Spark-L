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
#include "DataProc_Def.h"
#endif /* PKG_USING_LVGL */

#define NOVEL_INFO_MAX_LEN  (64)
#define FILE_NAME_MAX_LEN   (16)
#define FILE_PATH_MAX_LEN   (32)

const int MAX_DISP_PAGES = 1024 * 3;
const int MAX_TITLE_LEN = 32;
const int MAX_LINE_SIZE = LCD_W / (CHN_FONT_24x24 / 2);
const int MAX_FONTS_DISP = (LCD_W / CHN_FONT_24x24) * ((LCD_H - CHN_FONT_24x24) / CHN_FONT_24x24);
const int MAX_CHARS_DISP = (LCD_W / (CHN_FONT_24x24 / 2)) * ((LCD_H - CHN_FONT_24x24) / CHN_FONT_24x24);
const int MAX_BYTES_DISP = (LCD_W / (CHN_FONT_24x24 / 2)) * ((LCD_H - CHN_FONT_24x24) / CHN_FONT_24x24)
                           * (CHN_FONT_24x24 * CHN_FONT_32x32 / 8);

namespace Page
{
    class ReaderModel
    {
    public:
        struct NovelInfo
        {
            const char *Name;       // novel name
            const char *Path;       // novel file path
            off_t FileSize;      // novel file size
//            uint32_t TotalFonts;    // novel total font count
            uint32_t TotalPages;    // total novel pages (assemble)
//            uint32_t RealPages;     // calculate result
        };

        /* Page List */
        struct PageList
        {
            uint8_t status;

            uint16_t PageBytes;             // current total font number in one page
            char Context[MAX_CHARS_DISP];   // MAX contain chinese font
        };

    public:
        ReaderModel();
        ~ReaderModel();

    public:
        void Init();
        void Deinit();

        off_t getFileSize(int fd);
        uint16_t getPageFontCount(PageList pagelist, char *buf);
        uint32_t getTotalPages(int fd);

        bool loadNovel(const char *path, const char *name);
        int readNovel(int fd);
        void toNextPage();
        void toPrevPage();

        bool isLoaded(const char *name) const; // judge is load by novel's name
        bool isBusy();

    public:
        PageList *_PageList;  // page list for whole novel
        uint32_t m_nOldIndex;
        uint32_t m_nCurrentIndex;
        NovelInfo *m_NovelInfo;

    private:
        PageList *_Cache;
#ifdef PKG_USING_LVGL
        Account *account;
#endif
    };
}

#endif
