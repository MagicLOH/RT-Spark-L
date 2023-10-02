#ifndef __READER_H
#define __READER_H

//#include "ReaderView.h"
//#include "ReaderModel.h"

#include <rtthread.h>
#include <rtdef.h>
#include <vector>
#include <unistd.h>
#include <dfs.h>


class Reader
{
    /* File Info */
    typedef struct
    {
        const char *Name;       // file name
        const char *Path;       // file path
        uint32_t FileSize;      // file size
//            uint32_t TotalFonts;    // novel total font count
        uint32_t TotalPages;    // total file pages (assemble)
//            uint32_t RealPages;     // calculate result
    } FileInfo_t;

    /* Page List */
    typedef struct
    {
        bool status;
        uint16_t PageBytes;
        const char *Context;
    } PageList_t;

public:
    Reader();
    ~Reader();

    bool Load(const char *path, const char *name);

    void Next();

    int Read();

    bool Select(const char *name);

    bool isLoad(const char *name);

    off_t getFileSize();

public:
    std::vector<PageList_t> *m_PageList;

    FileInfo_t *m_FileInfo;

};

#endif

