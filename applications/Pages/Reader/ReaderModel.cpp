#include "ReaderModel.h"

#include <rtthread.h>
#include <unistd.h>
#include <dfs.h>

#define DBG_TAG "NovelModel"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

using namespace Page;


ReaderModel::ReaderModel()
    : _RootNode(nullptr)
    , novel_info(new NovelInfo)
{

}

ReaderModel::~ReaderModel()
{
    delete novel_info;
}

ReaderModel::PageList *ReaderModel::loadNovel(const char *path, const char *name)
{
    if (_RootNode == nullptr)
        return _RootNode;
    PageList *pnode = _RootNode;

    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        LOG_E("open novel failed! please check path!");
        return nullptr;
    }
    uint32_t file_size = getNovelFileSize(path);
    novel_info->FileSize = file_size;
    novel_info->Path = path;
    novel_info->Name = name;


    uint32_t total = 0;
    ssize_t rx = 0;
    uint32_t index = 1;
    char buf[MAX_FONT_COUNT] = {0};
    while (total < file_size)
    {
        rx = read(fd, buf, MAX_FONT_COUNT);
        if (rx < 0)
        {
            LOG_E("read novel interrupt!");
            close(fd);
            return nullptr;
        }
        memcpy(pnode->Context, buf, rx);

        pnode->PageIndex = index;     // update page index
        pnode = pnode->next;    // point to next page
        total += rx;
        index++;
    }

    close(fd);
    return _RootNode;
}

ReaderModel::PageList *ReaderModel::getPage(uint32_t index)
{
    uint32_t curr_index = 0;

    if (_RootNode == nullptr)
        return nullptr;

    PageList *pnode = _RootNode;

    while (pnode || curr_index != index)
    {
        pnode = pnode->next;
        curr_index++;
    }

    return pnode;
}

ReaderModel::PageList *ReaderModel::toNextPage()
{
    if (_RootNode == nullptr)
        return nullptr;

    PageList *node = _RootNode;


    return _RootNode;
}
ReaderModel::PageList *ReaderModel::toPrevPage()
{

    return _RootNode;
}

uint32_t ReaderModel::getNovelFileSize(const char *path)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        LOG_E("open novel failed! please check path!");
        return RT_EINVAL;
    }
    struct stat sbuf = {0};
    fstat(fd, &sbuf);
    off_t size = sbuf.st_size;

    close(fd);
    return sbuf.st_size > 0 ? size : 0;
}

uint16_t ReaderModel::getAPageFontCount()
{
    return 0;
}



