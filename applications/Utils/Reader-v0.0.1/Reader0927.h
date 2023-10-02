#ifndef __READER_H
#define __READER_H

#include <iostream>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DEBUG

#define _LOG(fmt, ...) printf("[Rdr" fmt "\r\n", ##__VA_ARGS__)
#define LOG_I(fmt, ...) _LOG("/Info] " fmt, ##__VA_ARGS__)
#define LOG_E(fmt, ...) _LOG("/Error/Ln %d] " fmt, __LINE__, ##__VA_ARGS__)
#define LOG_W(fmt, ...) _LOG("/Warn/Ln %d] " fmt, __LINE__, ##__VA_ARGS__)
#if defined(DEBUG)
#define LOG_D(fmt, ...) _LOG("/Debug/Ln %d] " fmt, __LINE__, ##__VA_ARGS__)
#else
#define LOG_D(fmt, ...)
#endif

#ifndef USING_USER_ASSERT
#include <assert.h>
#define rdr_assert(x) assert(x)
#endif

#define LCD_W (240)
#define LCD_H (240)
#define CHN_FONT_24x24 (24)

#define MAX_CHARS_DISP ((LCD_W / (CHN_FONT_24x24 / 2)) * \
                        (LCD_H - CHN_FONT_24x24) / (CHN_FONT_24x24))

#define MAX_LINE_SIZE ((LCD_W) / (CHN_FONT_24x24 / 2))

namespace _Reader
{
    typedef enum
    {
        TXT_TYPE = 1,
        EPUB_TYPE,
        MOBI_TYPE,
        AZW3_TYPE,
    } _BookType_t;

    typedef struct
    {
        const char *Path;
        const char *Name;
        _BookType_t Type;
    } _Bookshelf_t;
}

class Reader
{
public:
    typedef short ID_t;

    typedef struct
    {
        bool Status;
        char Content[MAX_CHARS_DISP];
        uint8_t APageBytes;
        uint16_t PageNumber;
    } PageInfo_t;

    typedef struct
    {
        ID_t ID; ///< bounded with list index(0,1,2,...)
        const char *Path;
        const char *Name;
        uint32_t TotalPages;
        uint32_t FileSize;

        std::vector<PageInfo_t *> Pagelist;
    } NovelInfo_t;

public:
    Reader();

    ~Reader();

    /* Scan a directory */
    bool Scan(const char *dir);

    /* Load a new novel */
    bool Load(const char *path, const char *name);

    /* List all novel infos */
    void Print();

    /* Go to next page */
    void Next();

    /* Go to prev page */
    void Prev();

    /* Go to indicate page */
    // TODO

    /* return a page load status */
    bool isLoad(const char *name);

    /* Set a page on viewing */
    void setPageOnView();

    /* Select a novel from novel stack */
    NovelInfo_t Select(const char *name);

private:
    /* Read whole content of a novel file */
    int Read(NovelInfo_t &novel);

    /* Add one page into page list */
    bool Add(const char *src, uint16_t count);

    /* get a file size */
    off_t getFileSize(const char *path);

public:
    std::vector<PageInfo_t> mPageList; //!< operate instance(cache)

    std::vector<NovelInfo_t> mNovelList; //!< operate instance

private:
    bool isFirst = true;
};

#endif