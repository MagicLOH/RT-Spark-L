#include "Reader.h"

using namespace _Reader;

#include <rtthread.h>
#define DBG_TAG "Reader"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

static _BookShelf_t bookshelf[] = {
	{"bullshit.txt", "bullshit", TXT_TYPE},
	{"sanguo.txt",   "sanguo",   TXT_TYPE},
};


Reader::Reader(const char *url)
{
	if (url)
	{
		/* Login epub website */
		// TODO
	}
}

Reader::~Reader()
{
}

/**
 * @brief Scan a directory's novels
 *
 * @param dir
 * @return true
 * @return false
 */
bool Reader::Scan(const char *dir_name)
{
	DIR *dirp = opendir(dir_name);
	if (!dirp)
	{
		LOG_I("Open dir failed!\n");
		return false;
	}

	struct dirent *dp = NULL;
	LOG_I("< %-15s > ================================================", dir_name);
	LOG_I("| %-15s | %10s | %10s | %-25s ",
		  "d_type", "d_reclen", "name len", "file name");
	LOG_I("--------------------------------------------------------------------");
	while (NULL != (dp = readdir(dirp)))
	{
		char type[64] = {0};
		switch (dp->d_type)
		{
			case DT_DIR:
				snprintf(type, sizeof(type), "%s", "Directory");
				break;
			case DT_REG:
				snprintf(type, sizeof(type), "%s", "Regular File");
				break;
			default:
				snprintf(type, sizeof(type), "%s", "Unknown Type");
				break;
		}
		LOG_I("| %-15s | %10u | %10ld | %-25s ",
			  type, dp->d_reclen, strlen(dp->d_name), dp->d_name);
	}
	LOG_I("====================================================================");

	closedir(dirp);
	return true;
}

/**
 * @brief Select a novel form novel list
 *
 * @param name
 * @return return a pointer to the m_NovelList's element
 */
Reader::NovelInfo_t Reader::Select(const char *name)
{
	NovelInfo_t nvl = {
		.ID = -1,
		.Type = UNKOWN_TYPE,
	};
	// memset(&nvl, 0, sizeof(NovelInfo_t));

	if (!(this->isLoad(name)))
	{
		LOG_W("%s was not loaded!", name);
		return nvl;
	}

	for (auto iter: m_NovelList)
	{
		if (strcmp(name, iter.Name) == 0)
			return iter;
	}

	return nvl;
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
	r_assert(path);
	r_assert(name);

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

	/* Copy parameters */
	NovelInfo_t novel;
	memset(&novel, 0, sizeof(NovelInfo_t));
	novel.ID = m_NovelList.size();
	novel.Path = path;
	novel.Name = name;
	novel.FileSize = getFileSize(novel.Path);
	novel.TotalPages = Read(novel);
	novel.Pagelist.swap(m_PageCache);

	m_NovelList.push_back(novel);
	isFirst = false;
	m_nCurrnetID = 0;
	m_nCurrentPageNum = 0;
	LOG_I("<%d:%s> load done.", novel.ID, novel.Name);

	LOG_D("========================== Load ==========================");
	LOG_D("m_PageCache.size() = %ld", m_PageCache.size());
	LOG_D("m_NovelList[%d].Pagelist.size() = %ld", novel.ID, m_NovelList[novel.ID].Pagelist.size());
	for (auto iter = m_NovelList[novel.ID].Pagelist.begin();
		 iter != m_NovelList[novel.ID].Pagelist.end();
		 ++iter)
	{
		LOG_D("[%d, %d]%s", iter->PageNumber, iter->APageBytes, iter->Content);
	}
	LOG_D("==========================================================");

	HAL::LCD_ShowString(0, CHN_FONT_24x24, CHN_FONT_24x24, "%s",
						m_NovelList.at(0).Pagelist.at(0).Content);

	close(fd);
	return true;
}

bool Reader::isLoad(const char *name)
{
	if (m_NovelList.empty() && isFirst == false)
	{
		LOG_W("The Novel list is empty!");
		return false;
	}

	for (auto iter: m_NovelList)
	{
		if (strcmp(name, iter.Name) == 0)
			return true;
	}

	return false;
}

/**
 * @brief
 *
 * @param buf
 * @param old_size
 * @return int
 */
int Reader::HalfWordFilter(uint8_t *buf, uint16_t old_size)
{
	uint8_t *p = buf;

	int new_size = 0;
	while (new_size < old_size && *p)
	{
		// example: (xx)111abcde(xx)1?
		// read at buf' end
		if (*p >= 0x80 && *(p + 1) == '\0')
		{
			// p += 2;
			// continue;
			break;
		}
		if (*p >= 0x80)
		{
			p += 2;
			new_size += 2;
			continue;
		}
		++p;
		++new_size;
	}

	// LOG_D("buf[0]=0x%02X", buf[0]);
	// LOG_D("buf[1]=0x%02X", buf[1]);
	// LOG_D("buf[2]=0x%02X", buf[2]);
	// LOG_D("buf[3]=0x%02X", buf[3]);
	// LOG_D("buf[old_size-1]=0x%02X", buf[old_size - 1]);
	// LOG_D("buf[old_size-1]=%d", buf[old_size - 1]);

	// LOG_D("===================== Filter =====================");
	// char tmp[MAX_CHARS_DISP + 1] = {0};
	// strncpy(tmp, (const char *)buf, old_size);
	// LOG_D("Before filter=%s\r\nold_size=%d", tmp, old_size);
	// memset(tmp, 0, MAX_CHARS_DISP + 1);
	// strncpy(tmp, (const char *)buf, new_size);
	// LOG_D("After filter=%s\r\nnew_size=%d", tmp, new_size);
	// LOG_D("==================================================");

	return new_size; // update new old_size
}

uint16_t Reader::DispSizeCalculator(uint8_t *src, const uint16_t disp_max_line,
									const uint16_t line_size_max)
{
	uint16_t disp_need = 0;
	uint16_t disp_line = 0;
	uint16_t line_count = 0;

	uint8_t *p = src;
	while (*p && disp_line < disp_max_line)
	{
		// ab\r\nabcdef
		if ((*p == '\r' && *(p + 1) == '\n')) // windows(CR+LF)
		{
			p += 2;
			disp_need += 2;
			disp_line += 1; // already display one line
			line_count = 0;
			continue;
		}
		else if (*p == '\n' || *p == '\r') // unix(LF) or mac(CR)
		{
			++p;
			++disp_need;
			++disp_line; // already display one line
			line_count = 0;
			continue;
		}
		// else if (*p >= 0x80 && ((line_count + 1) == line_size_max)) // the last char of each line
		// {
		//     // have not enough space to display an integrate chinese font
		//     ++disp_need;
		//     ++disp_line; // already display one line
		//     line_count = 0;
		//     continue;
		// }

		if (*p < 0x80)
		{
			++p;
			++disp_need;
			++line_count;
		}
		else
		{
			if ((line_count + 1) == line_size_max)
			{
				line_count = 0;
				++disp_line;
				continue;
			}
			p += 2;
			disp_need += 2;
			line_count += 2;
		}

		if (line_count == line_size_max)
		{
			line_count = 0;
			++disp_line; // already display one line
			// LOG_D("disp_line=%d", disp_line);
		}

	}
	// LOG_D("disp_line=%d", disp_line);

	/* Font Filter, drop half chinese and update new size */
	char tmp[MAX_CHARS_DISP + 1] = {0};
	strncpy(tmp, (const char *)src, disp_need);
//	LOG_D("disp_need=%d", disp_need);
	disp_need = HalfWordFilter((uint8_t *)tmp, disp_need);

	// LOG_D("=============== DISP NEED ===============");
	// char tmp[180] = {0};
	// strncpy(tmp, (const char *)src, disp_need);
	// LOG_D("tmp=%s", tmp);
	// LOG_D("disp_need=%d", disp_need);
	// LOG_D("=========================================");

	return disp_need;
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

	off_t offset_next = 0;  // store every read's count for necessary
	uint16_t disp_need = 0; // how many characters need to display on lcd

	uint8_t buf[MAX_CHARS_DISP + 1] = {0}; //!< take care of chinese characters
	// LOG_D("file size = %d ", novel.FileSize);
	for (uint32_t total = 0; total != novel.FileSize;)
	{
		lseek(fd, offset_next, SEEK_SET); // begin with last page's tail
		memset(buf, 0, sizeof(buf));
		/* read fixed length to fill buf */
		int rds = read(fd, buf, sizeof(buf) - 1);
		if (rds < 0)
		{
			LOG_E("read novel file interrupt!");
			close(fd);
			return -2;
		}
		buf[rds] = '\0';

		/* Font Filter, drop half chinese and update rds */
		// int rds_new = HalfWordFilter(buf, rds);
		// if (rds_new < 0)
		// {
		//     // TODO
		// }

		/*
		 * Calculate needed characters (disp_need) to display and
		 * deal with the '\r' or '\n' symbol.
		 */
		disp_need = DispSizeCalculator(buf,
									   MAX_COLUMN_SIZE,
									   MAX_LINE_SIZE);

		/* Add one page into the page list */
		if (!Add((const char *)buf, disp_need))
		{
			LOG_E("Add pages interrupt!");
			close(fd);
			return -3;
		}

		/* Common update index */
		offset_next += disp_need; // update really need offset length
		total += disp_need;       // remember update total size to end loop
		// LOG_D("offset_need = %d", offset_next);
		// LOG_D("total=%d", total);
	}

	/* Recheck every page */
	// TODO

	// LOG_D("m_PageCache->size() = %d", m_PageCache.size());
	close(fd);
	return m_PageCache.size();
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

	char buf[MAX_CHARS_DISP + 1] = {0};
	strncpy(buf, src, count);
	strcpy(pagelist.Content, buf);

	// LOG_D("=========================Add===============================");
	// LOG_D("pagelist.Content = %s", pagelist.Content);
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
		pagelist.PageNumber = m_PageCache.size() + 1;
		m_PageCache.push_back(pagelist);
	}

	return pagelist.Status;
}

/**
 * @brief set the top novel of list on viewing
 * @return none
 */
void Reader::setPageOnView(uint16_t NovelID, uint16_t PageNumber)
{
	// example:
	// cout << m_PageCache->at(0).Content << endl;
	uint8_t *p = (uint8_t *)m_NovelList.at(NovelID).Pagelist.at(PageNumber).Content;
	uint8_t line_size = MAX_LINE_SIZE;

	for (int i = 0; i < MAX_LINE_SIZE; ++i)
	{
		rt_kprintf("=");
	}
	rt_kprintf("\r\n");
	while (*p)
	{
		if ((*p == '\r' && *(p + 1) == '\n')) // windows(CR+LF)
		{
			p += 2;
			line_size = MAX_LINE_SIZE;
			rt_kprintf("\r\n");
			continue;
		}
		else if (*p == '\n' || *p == '\r') // unix(LF) or mac(CR)
		{
			p++;
			line_size = MAX_LINE_SIZE;
			rt_kprintf("\r\n");
			continue;
		}
		else if (*p > 0x80 && ((line_size - 1) == 0))
		{
			line_size = MAX_LINE_SIZE;
			rt_kprintf(" \r\n");
			continue;
		}

		if (!line_size)
		{
			line_size = MAX_LINE_SIZE;
			rt_kprintf("\r\n");
		}

		if (*p < 0x80)
		{
			rt_kprintf("%c", *p);
			p++;
			--line_size;
		}
		else
		{
			rt_kprintf("%c%c", *p, *(p + 1));
			p += 2;
			line_size -= 2;
		}
	}

	rt_kprintf("\r\n");
	for (int i = 0; i < MAX_LINE_SIZE; ++i)
	{
		rt_kprintf("=");
	}
	rt_kprintf("\r\n");
}

/**
 * @brief Go to next page
 *
 */
void Reader::Next()
{
	m_nCurrentPageNum++;
	if (m_nCurrentPageNum >= m_NovelList.at(m_nCurrnetID).Pagelist.size())
		m_nCurrentPageNum = 0;
	// do something...
	HAL::LCD_Clean(0x0000);
	HAL::LCD_ShowString(0, CHN_FONT_24x24, CHN_FONT_24x24, "%s",
						m_NovelList.at(m_nCurrnetID).Pagelist.at(m_nCurrentPageNum).Content);
}

/**
 * @brief Go to prev page
 *
 */
void Reader::Prev()
{
	m_nCurrentPageNum--;
	if (m_nCurrentPageNum <= 0)
		m_nCurrentPageNum = m_NovelList.at(m_nCurrnetID).Pagelist.size() - 1;
	// do something...
	HAL::LCD_Clean(0x0000);
	HAL::LCD_ShowString(0, CHN_FONT_24x24, CHN_FONT_24x24, "%s",
						m_NovelList.at(m_nCurrnetID).Pagelist.at(m_nCurrentPageNum).Content);
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

	for (auto iter: m_NovelList)
	{
		LOG_I("| %-*.*s | %-*.*s | %08d B | %08d B |",
			  FILE_NAME_MAX_LEN, NOVEL_INFO_MAX_LEN, iter.Name,
			  FILE_PATH_MAX_LEN, NOVEL_INFO_MAX_LEN, iter.Path,
			  iter.FileSize,
			  iter.TotalPages);
	}

	LOG_I("==================================================================================");
}