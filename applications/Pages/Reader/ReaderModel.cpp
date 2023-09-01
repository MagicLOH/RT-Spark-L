#include "ReaderModel.h"

#include <rtthread.h>
#include <unistd.h>
#include <dfs.h>
#include "DataProc.h"

#define DBG_TAG "ReaderModel"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

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
	delete _RootList;
	if (account)
	{
		delete account;
		account = nullptr;
	}
}

void ReaderModel::Init()
{
	account = new Account("ReaderModel", DataProc::Center(), 0, this);
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
		LOG_W("%s is already loaded!", name);
		return false;
	}

	int fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		LOG_E("open novel failed! please check path!");
		return false;
	}
	uint32_t file_size = getFileSize(fd);
	RT_ASSERT(m_NovelInfo);
	m_NovelInfo->FileSize = file_size;
	m_NovelInfo->Path = path;
	m_NovelInfo->Name = name;
	m_NovelInfo->TotalFonts = file_size / 2; // GBK font, one character = two byte
	m_NovelInfo->TotalPages = m_NovelInfo->TotalFonts / MAX_FONTS_DISP;

	/* Init Root List */
	_RootList = new PageList[m_NovelInfo->TotalPages];
	RT_ASSERT(_RootList);

	uint32_t total = 0;
	ssize_t rx = 0;
	uint32_t index = 0;
	m_NovelInfo->RealPages = 0;
	char buf[MAX_FONTS_DISP] = { 0 };
	while (total < file_size && index < m_NovelInfo->TotalPages)
	{
		rx = read(fd, buf, MAX_FONTS_DISP);
		if (rx < 0)
		{
			LOG_E("read novel file interrupt!");
			close(fd);
			return false;
		}
		LOG_D("rx = %d", rx);
		rt_memcpy(_RootList[index].Context, buf, rx);
		_RootList[index].FontCount = rx;

		m_NovelInfo->RealPages++;
		index++;
		total += rx;
	}
	LOG_D("index = %d, realPages = %d", index, m_NovelInfo->RealPages);

	close(fd);
	return true;
}

ReaderModel::PageList *ReaderModel::getPage(uint32_t index)
{

	return nullptr;
}

void ReaderModel::toNextPage()
{
	if (_RootList)
	{
		LOG_D("root list had not be created!");
		return;
	}

	m_nOldIndex = m_nCurrentIndex;
	if (m_nCurrentIndex >= 0)
	{
		m_nCurrentIndex++;
	}
	if (m_nCurrentIndex != m_nOldIndex)
	{
		lcd_show_string(0,
		                CHN_FONT_24x24,
		                CHN_FONT_24x24,
		                "%s",
		                _RootList[m_nCurrentIndex].Context);
	}
}

void ReaderModel::toPrevPage()
{
	if (_RootList)
	{
		LOG_D("root list had not be created!");
		return;
	}
	if (m_nCurrentIndex != m_nOldIndex)
	{
		m_nOldIndex = m_nCurrentIndex;
		if (m_nCurrentIndex > 0)
		{
			m_nCurrentIndex--;
		}
		lcd_show_string(0,
		                CHN_FONT_24x24,
		                CHN_FONT_24x24,
		                "%s",
		                _RootList[m_nCurrentIndex].Context);
	}

}

uint32_t ReaderModel::getFileSize(int fd)
{
	if (fd < 0)
	{
		LOG_E("get file size failed! check fd!");
		return RT_EINVAL;
	}
	struct stat sbuf = { 0 };
	fstat(fd, &sbuf);
	off_t size = sbuf.st_size;

	return sbuf.st_size > 0 ? size : 0;
}

uint16_t ReaderModel::getAPageFontCount()
{
	return _RootList ? _RootList->FontCount : 0;
}

bool ReaderModel::isLoaded(const char *name) const
{
	if (0 == rt_strcmp(name, m_NovelInfo->Name)
	    && nullptr != _RootList
	    && nullptr != m_NovelInfo->Name)
	{
		LOG_I("%s was already loaded.", name);
		return true;
	}

	return false;
}




