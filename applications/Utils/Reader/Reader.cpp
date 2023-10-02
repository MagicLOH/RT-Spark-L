#include "Reader.h"

#include "HAL.h"

#define DBG_TAG "Reader"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define LCD_W (240)
#define LCD_H (240)
#define NOVEL_INFO_MAX_LEN  (64)
#define FILE_NAME_MAX_LEN   (16)
#define FILE_PATH_MAX_LEN   (32)

const int MAX_TITLE_LEN = 32;
const int MAX_LINE_SIZE = LCD_W / (CHN_FONT_24x24 / 2);
const int MAX_FONTS_DISP = (LCD_W / CHN_FONT_24x24) * ((LCD_H - CHN_FONT_24x24) / CHN_FONT_24x24);
const int MAX_CHARS_DISP = (LCD_W / (CHN_FONT_24x24 / 2)) * ((LCD_H - CHN_FONT_24x24) / CHN_FONT_24x24);


Reader::Reader()
    : m_PageList(new std::vector<PageList_t>)
{

}

Reader::~Reader()
{

}

bool Reader::Load(const char *path, const char *name)
{
    RT_ASSERT(path);
    RT_ASSERT(name);

    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        LOG_E("open novel failed! please check path!");
        return false;
    }

    RT_ASSERT(m_FileInfo);
    m_FileInfo->Path = path;
    m_FileInfo->Name = name;
    m_FileInfo->FileSize = getFileSize();
    m_FileInfo->TotalPages = Read();

    LOG_I("================================= Novel Info ==============================");
    LOG_I("| %-*.*s | %-*.*s |  FileSize  | TotalPages |",
          FILE_NAME_MAX_LEN, NOVEL_INFO_MAX_LEN, "FileName",
          FILE_PATH_MAX_LEN, NOVEL_INFO_MAX_LEN, "FilePath");
    LOG_I("----------------------------------------------------------------------------------");
    LOG_I("| %-*.*s | %-*.*s | 0x%08lX | 0x%08lX |",
          FILE_NAME_MAX_LEN, NOVEL_INFO_MAX_LEN, m_FileInfo->Name,
          FILE_PATH_MAX_LEN, NOVEL_INFO_MAX_LEN, m_FileInfo->Path,
          m_FileInfo->FileSize,
          m_FileInfo->TotalPages);
    LOG_I("===========================================================================");

    close(fd);
    return true;
}

off_t Reader::getFileSize()
{
    int fd = open(m_FileInfo->Path, O_RDONLY);
    if (fd < 0)
    {
        LOG_E("get file size failed! check fd!");
        return RT_EINVAL;
    }
    struct stat sbuf = {0};
    fstat(fd, &sbuf);
    off_t size = sbuf.st_size;

    close(fd);
    return size > 0 ? size : 0;
}


int Reader::Read()
{
    int index = 0;            // mark current page list context's index
    uint32_t disp_need;       // how many characters need to display on lcd
    uint32_t offset_need = 0; // store every read's count for necessary
    uint8_t page_remain;
    uint8_t line_remain;      // one line's remain character(byte)

    int fd = open(m_FileInfo->Path, O_RDONLY);
    if (fd < 0)
    {
        LOG_E("open <%s> failed!", m_FileInfo->Path);
        return -1;
    }

    char buf[MAX_CHARS_DISP] = {0};
    char *p = RT_NULL;
    /* Loop Reading */
    ssize_t rds = 0;
    for (uint32_t total = 0; total < m_FileInfo->FileSize; total += rds)
    {
        lseek(fd, (off_t)offset_need, SEEK_SET); // begin with last page's tail
        memset(buf, 0, sizeof(buf));
        rds = read(fd, buf, sizeof(buf) - 1); // read fix length to fill buf
        if (rds < 0)
        {
            LOG_E("read file interrupt!");
            close(fd);
            return -2;
        }
        buf[rds] = '\0';
        LOG_D("read size = %d", rds);

        p = buf;
        disp_need = 0;
        page_remain = MAX_CHARS_DISP;
        line_remain = MAX_LINE_SIZE;

        /* Calculate needed characters to display */
        while (*p && page_remain > 0 && disp_need < rds)
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

            p++;
            disp_need++;
            page_remain--;
            if (!line_remain)
                line_remain = MAX_LINE_SIZE;
            else
                line_remain--;
        }
        LOG_D("disp_need = %d", disp_need);

        /* Fill each list */
        PageList_t pageList = {0};
        pageList.Context = (const char *)buf; // force buf to const variety
        LOG_D("strlen(pageList.Context) = %d", strlen(pageList.Context));
        if (disp_need != strlen(pageList.Context))
        {
            pageList.status = false;
            LOG_E("Read data have problems!");
            close(fd);
            return -3;
        }
        pageList.PageBytes = disp_need;
        pageList.status = true;
        m_PageList->push_back(pageList);


        offset_need += disp_need;
        LOG_D("offset_need = %d", offset_need);
    }

//    HAL::LCD_ShowString(0,
//                        CHN_FONT_24x24,
//                        CHN_FONT_24x24,
//                        "%s",
//                        m_PageList[0].data()->Context);

    rt_kprintf("%s\n", m_PageList[0].data()->Context);
    rt_kprintf("%s\n", m_PageList[1].data()->Context);
    rt_kprintf("%s\n", m_PageList[2].data()->Context);

    close(fd);
    return (int)m_PageList->size();
}

