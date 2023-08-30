#ifndef __READER_MODEL_H
#define __READER_MODEL_H

#ifdef PKG_USING_LVGL
#include "lvgl.h"
#endif

extern "C"
{
#include "drv_st7789.h"
}

#include "HAL.h"

#define MAX_FONT_COUNT (LCD_W * (LCD_H - CHN_FONT_24x24) / CHN_FONT_24x24)

namespace Page
{
    class ReaderModel
    {
    public:
        typedef enum
        {
            EVENT_NEXT_PAGE, // go to next page
            EVENT_PREV_PAGE, // go to preview page
            EVENT_EXIT       // exit to novel library
        } EventCode_t;

        struct NovelInfo
        {
            const char *Name;       // novel name
            const char *Path;       // novel file path
            uint32_t FileSize;      // novel file size
        };

        /* Page List */
        struct PageList
        {
            uint32_t PageIndex;             // current page index
            uint16_t FontCount;             // current total font number in one page
            char Context[MAX_FONT_COUNT];   // MAX contain chinese font
            struct PageList *next;
            struct PageList *prev;
        };

        /* Event call back pointer */
        typedef void (*EventCallback_t)(ReaderModel *novel);

    public:
        ReaderModel::ReaderModel();
        ~ReaderModel();

    public:
        uint32_t getNovelFileSize(const char *path);
        uint16_t getAPageFontCount();
        PageList *ReaderModel::getPage(uint32_t index);

        PageList *loadNovel(const char *path, const char *name);
        PageList *toNextPage();
        PageList *toPrevPage();

    private:
        PageList *_RootNode;  // root node for whole novel
        NovelInfo *novel_info;

    };
}

#endif
