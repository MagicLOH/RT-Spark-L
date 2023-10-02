#include "Reader.h"

using namespace _Reader;

static _Bookshelf_t BookShelf[] = {
    {"bullshit.txt", "bullshit", TXT_TYPE},
};

Reader::Reader()
{
}

Reader::~Reader()
{
}

/**
 * @brief Scan a directory's novels, 
 *        then write to bookshelf
 *
 * @param dir
 * @return true
 * @return false
 */
bool Reader::Scan(const char *dir)
{
    // TODO

    return false;
}

/**
 * @brief print novel info
 *
 */
void Reader::Print()
{
#define FILE_NAME_MAX_LEN (16)
#define FILE_PATH_MAX_LEN (32)
#define NOVEL_INFO_MAX_LEN (32)
    LOG_I("================================= About Novel ====================================");
    LOG_I("| %-*.*s | %-*.*s |  FileSize  | TotalPages |",
          FILE_NAME_MAX_LEN, NOVEL_INFO_MAX_LEN, "FileName",
          FILE_PATH_MAX_LEN, NOVEL_INFO_MAX_LEN, "FilePath");
    LOG_I("----------------------------------------------------------------------------------");

    for (auto iter : mNovelList)
    {
        LOG_I("| %-*.*s | %-*.*s | %08d B | %08d B |",
              FILE_NAME_MAX_LEN, NOVEL_INFO_MAX_LEN, iter.Name,
              FILE_PATH_MAX_LEN, NOVEL_INFO_MAX_LEN, iter.Path,
              iter.FileSize,
              iter.TotalPages);
    }

    LOG_I("==================================================================================");
}

/**
 * @brief Load a novel
 *
 * @param path
 * @param name
 * @return true
 * @return false
 */
bool Reader::Load(const char *path, const char *name)
{
    rdr_assert(path);
    rdr_assert(name);

    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        LOG_E("open novel failed! please check path!");
        return false;
    }

    if (isLoad(name))
    {
        LOG_E("%s is already loaded!", name);
        return false;
    }

    NovelInfo_t novel = {0};
    novel.ID = mNovelList.size();
    novel.Path = path;
    novel.Name = name;
    novel.FileSize = getFileSize(novel.Path);
    novel.TotalPages = Read(novel);

    mNovelList.push_back(novel);
    isFirst = false;
    LOG_I("<%d:%s> load done.", novel.ID, novel.Name);

    LOG_D("===========================================================");
    for (auto iter = mPageList.begin();
         iter != mPageList.end();
         ++iter)
    {
        LOG_D("%d, %s", iter->PageNumber, iter->Content);
        // LOG_D("%d, %p", iter->PageNumber, iter);
    }
    LOG_D("===========================================================");

    close(fd);
    return true;
}

bool Reader::isLoad(const char *name)
{
    if (mNovelList.empty() && isFirst == false)
    {
        LOG_W("The Novel list is empty!");
        return false;
    }

    for (auto iter : mNovelList)
    {
        if (strcmp(name, iter.Name) == 0)
            return true;
    }

    return false;
}

/**
 * @brief Read one book
 *
 * @param novel
 * @return int return the novel's total pages
 */
int Reader::Read(NovelInfo_t &novel)
{
    int fd = open(novel.Path, O_RDONLY);
    if (fd < 0)
    {
        LOG_E("open %s failed!", novel.Path);
        return -1;
    }

    off_t offset_next = 0;     // store every read's count for necessary
    uint16_t disp_need = 0;    // how many characters need to display on lcd
    uint8_t screen_remain = 0; // calculate current screen remain space
    uint8_t line_remain = 0;   // one line's remain characters(byte)

    uint8_t buf[MAX_CHARS_DISP] = {0}; //!< take care of chinese characters
    // LOG_D("file size = %d ", novel.FileSize);
    for (uint32_t total = 0; total < novel.FileSize;)
    {
        lseek(fd, offset_next, SEEK_SET); // begin with last page's tail
        memset(buf, 0, sizeof(buf));
        /* read fixed length to fill buf */
        int rds = read(fd, buf, sizeof(buf) - 1);
        if (rds < 0)
        {
            LOG_E("read novel file interrupt!");
            close(fd);
            return -1;
        }
        buf[rds] = '\0';

        // LOG_D("====================================");
        // LOG_D("rds = %d", rds);
        // LOG_D("read buf = %s", buf);
        // LOG_D("buf[rds-1] = %d", buf[rds - 1]);
        // LOG_D("buf[rds] = %d", buf[rds]);

        /* Font Filter, drop half chinese and update rds */
        uint8_t *p = buf;
        int cnt = 0;
        while (*p && cnt < rds) // example: (xx)111abcde(xx)1??
        {
            if (*p > 0x80 && *(p + 1) < 0x80)
            {
                // p += 2;
                // continue;
                break;
            }
            else if (*p < 0x80 || *p == '\r' || *p == '\n')
            {
                p++;
                cnt++;
                continue;
            }

            p += 2;
            cnt += 2;
        }
        p = nullptr; // to avoid pointer out of bounds
        rds = cnt;   // update new rds

        char tmp[MAX_CHARS_DISP] = {0};
        strncpy(tmp, (const char *)buf, rds);
        // LOG_D("tmp = %s", tmp);
        // LOG_D("cnt = %d", cnt);
        // LOG_D("rds = %d", rds);

        /*
         * Calculate needed characters to display and
         * deal with the '\r' or '\n' symbol
         */
        p = buf;                     // update pointer index
        disp_need = 0;               // start at 0
        screen_remain = rds;         // restore to needed size
        line_remain = MAX_LINE_SIZE; // restore to needed size
        while (*p && screen_remain > 0)
        {
            if ((*p == '\r' && *(p + 1) == '\n')) // unix(LF)
            {
                p += 2;
                disp_need += 2;
                screen_remain -= line_remain;
                line_remain = MAX_LINE_SIZE; // restore remain size
                continue;
            }
            else if (*p == '\n') // or windows(CR+LF)
            {
                p++;
                disp_need++;
                screen_remain -= line_remain;
                line_remain = MAX_LINE_SIZE; // restore remain size
                continue;
            }

            p++;
            disp_need++;
            screen_remain--;
            if (!line_remain)
                line_remain = MAX_LINE_SIZE;
            else
                line_remain--;
        }

        /* Add one page into the page list */
        if (!Add((const char *)buf, disp_need))
        {
            LOG_E("Add pages interrupt!");
            close(fd);
            return -2;
        }

        offset_next += disp_need; // update really need offset length
        total += rds;             // remember update total to avoid infinite loop
        // LOG_D("offset_need = %d", offset_next);
    }

    /* Recheck every page */
    // TODO

    // LOG_D("mPageList->size() = %d", mPageList.size());
    close(fd);
    return mPageList.size();
}

/**
 * @brief Get novel file's size
 *
 * @param path
 * @return off_t
 */
off_t Reader::getFileSize(const char *path)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        LOG_E("get file size failed! check fd!");
        return -1;
    }
    struct stat sbuf = {0};
    memset(&sbuf, 0, sizeof(struct stat));
    fstat(fd, &sbuf);
    off_t size = sbuf.st_size;

    return size > 0 ? size : 0;
}

/**
 * @brief Add one page into PageList
 *
 * @param src source content
 * @param count one page's content size
 * @return true
 * @return false
 */
bool Reader::Add(const char *src, uint16_t count)
{
    PageInfo_t pagelist;

    char buf[MAX_CHARS_DISP] = {0};
    strncpy(buf, src, count);
    strcpy(pagelist.Content, buf);

    // LOG_D("===========================================================");
    // LOG_D("Content = %s", pagelist.Content);
    // LOG_D("count = %d", count);
    // LOG_D("strlen(pagelist.Content) = %d", strlen(pagelist.Content));
    // LOG_D("===========================================================");

    if (count != strlen(pagelist.Content))
    {
        pagelist.Status = false;
        pagelist.APageBytes = 0;
        LOG_W("Add interrupt! Page's Content have problem!");
    }
    else
    {
        pagelist.Status = true;
        pagelist.APageBytes = count;
        pagelist.PageNumber = mPageList.size() + 1;
        mPageList.push_back(pagelist);
    }

    return pagelist.Status ? true : false;
}

/**
 * @brief Select a novel form novel list
 *
 * @param name
 * @return return a pointer to the mNovelList's element
 */
Reader::NovelInfo_t Reader::Select(const char *name)
{
    NovelInfo_t nvl;
    memset(&nvl, 0, sizeof(NovelInfo_t));

    if (!(this->isLoad(name)))
    {
        LOG_W("%s was not loaded!", name);
        return nvl;
    }

    for (auto iter : mNovelList)
    {
        if (strcmp(name, iter.Name) == 0)
        {
            nvl.Name = iter.Name;
            nvl.Path = iter.Path;
            nvl.ID = iter.ID;
            return nvl;
        }
    }

    return nvl;
}

/**
 * @brief set the top novel of list on viewing
 * @return none
 */
void Reader::setPageOnView()
{
    // example:
    // cout << mPageList->at(0).Content << endl;
}

/**
 * @brief Go to next page
 *
 */
void Reader::Next()
{
    // m_PageNumber++;
    // cout << mPageList.at(m_PageNumber)->Content << endl;
}

/**
 * @brief Go to prev page
 *
 */
void Reader::Prev()
{
    // m_PageNumber--;
    // cout << mPageList.at(m_PageNumber)->Content << endl;
}

int main()
{
    Reader *reader = new Reader;
    reader->Load(BookShelf[0].Path, BookShelf[0].Name);

    delete reader;
    return 0;
}
