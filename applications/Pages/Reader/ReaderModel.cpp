#include "ReaderModel.h"

#include <rtthread.h>
#include <unistd.h>
#include <dfs.h>

#ifdef PKG_USING_LVGL
#include "DataProc.h"
#endif
#define DBG_TAG "ReaderModel"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

extern "C" {
#include "drv_st7789.h"

}

using namespace Page;


/**
 * @brief reader model constructor
 */
ReaderModel::ReaderModel()
    : m_NovelInfo(new NovelInfo)
{
    rt_memset(m_NovelInfo, 0, sizeof(NovelInfo));

    m_nCurrentIndex = 0;
    m_nOldIndex = 0;
}

/**
 * @brief reader model destructor
 */
ReaderModel::~ReaderModel()
{
    delete m_NovelInfo;
    delete[] _Cache;
#ifdef PKG_USING_LVGL
    if (account)
    {
        delete account;
        account = nullptr;
    }
#endif
}

void ReaderModel::Init()
{
//    account = new Account("ReaderModel", DataProc::Center(), 0, this);
//    account->Subscribe("NovelReader");
}

void ReaderModel::Deinit()
{

}

/**
 * @brief when reader instance create, next step is load a novel
 * @param path
 * @param name
 * @return
 */
bool ReaderModel::loadNovel(const char *path, const char *name)
{
    RT_ASSERT(path);
    RT_ASSERT(name);
    if (isLoaded(name))
    {
        LOG_W("%s was already loaded!", name);
        return false;
    }

    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        LOG_E("open novel failed! please check path!");
        return false;
    }

    _Cache = new PageList[MAX_DISP_PAGES];
    RT_ASSERT(_Cache);

    off_t file_size = getFileSize(fd);
    RT_ASSERT(m_NovelInfo);
    m_NovelInfo->FileSize = file_size;
    m_NovelInfo->Path = path;
    m_NovelInfo->Name = name;
    m_NovelInfo->TotalPages = readNovel(fd);
    LOG_I("================================= Novel Information ==============================");
    LOG_I("| %-*.*s | %-*.*s |  FileSize  | TotalPages |",
          FILE_NAME_MAX_LEN, NOVEL_INFO_MAX_LEN, "FileName",
          FILE_PATH_MAX_LEN, NOVEL_INFO_MAX_LEN, "FilePath");
    LOG_I("----------------------------------------------------------------------------------");
    LOG_I("| %-*.*s | %-*.*s | 0x%08lX | 0x%08lX |",
          FILE_NAME_MAX_LEN, NOVEL_INFO_MAX_LEN, m_NovelInfo->Name,
          FILE_PATH_MAX_LEN, NOVEL_INFO_MAX_LEN, m_NovelInfo->Path,
          m_NovelInfo->FileSize,
          m_NovelInfo->TotalPages);
    LOG_I("==================================================================================");

//    rt_kprintf("%s\n", _Cache[0].Context);


    close(fd);
    return true;
}

void ReaderModel::toNextPage()
{
    if (_PageList)
    {
        LOG_D("root list had not be created!");
        return;
    }
#ifdef PKG_USING_LVGL
    DataProc::NovelReader_Info_t info;
    DATA_PROC_INIT_STRUCT(info);
    info.NovelName = m_NovelInfo->Name;
    info.cmd = DataProc::NOVEL_READER_CMD_SHOW;
    info.NovelContext = new char[MAX_FONTS_DISP];
#endif
    m_nOldIndex = m_nCurrentIndex;
    if (m_nCurrentIndex >= 0)
    {
        m_nCurrentIndex++; // update page index
    }
    if (m_nCurrentIndex != m_nOldIndex)
    {
#ifdef PKG_USING_LVGL
        rt_memcpy(info.NovelContext,
                  _RootList[m_nCurrentIndex].Context,
                  rt_strlen(_RootList[m_nCurrentIndex].Context));
        account->Notify("NovelReader", &info, sizeof(info));
#endif
        lcd_clear(BLACK);
        HAL::LCD_ShowString(0,
                            CHN_FONT_24x24,
                            CHN_FONT_24x24,
                            "%s",
                            _PageList[m_nCurrentIndex].Context);
    }
}

void ReaderModel::toPrevPage()
{
    if (_PageList)
    {
        LOG_D("root list had not be created!");
        return;
    }
#ifdef PKG_USING_LVGL
    DataProc::NovelReader_Info_t info;
    DATA_PROC_INIT_STRUCT(info);
    info.NovelName = m_NovelInfo->Name;
    info.cmd = DataProc::NOVEL_READER_CMD_SHOW;
    info.NovelContext = new char[MAX_FONTS_DISP];
#endif
    if (m_nCurrentIndex != m_nOldIndex)
    {
        m_nOldIndex = m_nCurrentIndex;
        if (m_nCurrentIndex > 0)
        {
            m_nCurrentIndex--; // update page index
#ifdef PKG_USING_LVGL
            rt_memcpy(info.NovelContext,
                      _RootList[m_nCurrentIndex].Context,
                      rt_strlen(_RootList[m_nCurrentIndex].Context));
            account->Notify("NovelReader", &info, sizeof(info));
#endif
            lcd_clear(BLACK);
            HAL::LCD_ShowString(CHN_FONT_24x24 * 2,
                                CHN_FONT_24x24,
                                CHN_FONT_24x24,
                                "%s",
                                _PageList[m_nCurrentIndex].Context);
        }

    }

}


off_t ReaderModel::getFileSize(int fd)
{
    if (fd < 0)
    {
        LOG_E("get file size failed! check fd!");
        return RT_EINVAL;
    }
    struct stat sbuf = {0};
    fstat(fd, &sbuf);
    off_t size = sbuf.st_size;

    return size > 0 ? size : 0;
}

uint32_t ReaderModel::getTotalPages(int fd)
{
    if (fd < 0)
    {
        LOG_E("fd has problem!");
        return 0;
    }
    uint32_t total = 0;


    return total;
}

uint16_t ReaderModel::getPageFontCount(PageList pagelist, char *buf)
{
    uint16_t max_count = MAX_CHARS_DISP;
    while (*buf)
    {
        if (*buf < 0x80) // is ascii
        {
            max_count--;
        }
        else // is chinese
        {
            max_count -= 2;
        }
    }


    return _PageList ? _PageList->PageBytes : 0;
}

bool ReaderModel::isLoaded(const char *name) const
{
    if (0 == rt_strcmp(name, m_NovelInfo->Name)
        && nullptr != _PageList
        && nullptr != m_NovelInfo->Name)
    {

        return true;
    }

    return false;
}

int ReaderModel::readNovel(int fd)
{
    int total = 0;
    int index = 0;                          // mark current page list context's index
    uint32_t disp_need;                     // how many characters need to display on lcd
    uint32_t offset_need = 0;               // store every read's count for necessary
    uint8_t page_remain;
    uint8_t line_remain;   // one line's remain character(byte)

    char buf[MAX_CHARS_DISP];
    char *p = RT_NULL;
    while (total < m_NovelInfo->FileSize)
    {
        lseek(fd, (off_t)offset_need, SEEK_SET);     // begin with last page's tail
        rt_memset(buf, 0x00, sizeof(buf));
        ssize_t rds = read(fd, buf, MAX_CHARS_DISP); // read fix length to fill buf
        if (rds < 0)
        {
            LOG_E("read novel file interrupt!");
            close(fd);
            return -1;
        }
        LOG_D("rx = %d", rds);

        p = buf;
        disp_need = 0;
        page_remain = MAX_CHARS_DISP;
        line_remain = MAX_LINE_SIZE;
        /* Calculate needed characters to display */
        while (*p && page_remain > 0)
        {
            if ((*p == '\r' && *(p + 1) == '\n')) // unix(LF)
            {
                p += 2;
                disp_need += 2;
                page_remain -= line_remain;
                line_remain = MAX_LINE_SIZE; // restore remain size
                continue;
            }
            else if (*p == '\n') // or windows(CR+LF)
            {
                p++;
                disp_need++;
                page_remain -= line_remain;
                line_remain = MAX_LINE_SIZE; // restore remain size
                continue;
            }
            if (!line_remain)
            {
                line_remain = MAX_LINE_SIZE;
            }

            p++;
            disp_need++;
            page_remain--;
            line_remain--;
        }
        rt_memset(_Cache[index].Context, 0x00, MAX_CHARS_DISP);
        LOG_D("disp_need = %d", disp_need);
        rt_memcpy(_Cache[index].Context, buf, disp_need); // only copy needed

        offset_need += disp_need;
        LOG_D("offset_need = %d", offset_need);
        index++;
        total += rds;
    }

    HAL::LCD_ShowString(0,
                        CHN_FONT_24x24,
                        CHN_FONT_24x24,
                        "%s",
                        _Cache[0].Context);
    return index;
}





