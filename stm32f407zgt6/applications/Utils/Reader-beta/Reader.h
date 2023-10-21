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
#include <dirent.h>

#include "hal.h"

#define DEBUG
#define READER_USING_LOG
#define USING_USER_ASSERT

#if defined(READER_USING_LOG) && !defined(RT_DEBUG)
#include <stdio.h>
#define _LOG(fmt, ...) printf("[Rdr" fmt "\r\n", ##__VA_ARGS__)
#define LOG_I(fmt, ...) _LOG("/Info] " fmt, ##__VA_ARGS__)
#define LOG_E(fmt, ...) _LOG("/Error/Ln %d] " fmt, __LINE__, ##__VA_ARGS__)
#define LOG_W(fmt, ...) _LOG("/Warn/Ln %d] " fmt, __LINE__, ##__VA_ARGS__)
#if defined(DEBUG)
#define LOG_D(fmt, ...) _LOG("/Debug/Ln %d] " fmt, __LINE__, ##__VA_ARGS__)
#else
#define LOG_D(fmt, ...)
#endif

#else

#endif

#ifndef USING_USER_ASSERT
#include <assert.h>
#define r_assert(x) assert(x)
#else
#define r_assert(x) RT_ASSERT(x)
#endif


namespace _Reader
{
typedef enum
{
	UNKOWN_TYPE = -1,
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
} _BookShelf_t;
}

using namespace _Reader;

class Reader
{
public:
	typedef short ID_t;

	typedef _BookType_t Type_t;

	typedef struct
	{
		bool Status;
		char Content[MAX_CHARS_DISP + 1];
		uint16_t APageBytes;
		uint16_t PageNumber;
	} PageInfo_t;

	typedef struct
	{
		ID_t ID;             // bounded with list index(0,1,2,...)
		Type_t Type;         // book file type
		const char *URL;     // url, ebook only
		const char *Path;    // basic file path
		const char *Name;    // novel name
		uint32_t TotalPages; // book's total pages
		uint32_t FileSize;   // book file size

		// unused so reserve
		std::vector<PageInfo_t> Pagelist;
	} NovelInfo_t;

public:
	Reader() = default;
	Reader(const char *url);

	virtual ~Reader();

	/* Scan a directory */
	bool Scan(const char *dir_name);

	/* Load a new novel */
	bool Load(const char *path, const char *name);

	/* List all novel infos */
	void Print();

	/* Go to next page */
	void Next();

	/* Go to prev page */
	void Prev();

	/* Go to indicate page */
	void Goto();
	// TODO

	/* return a page load status */
	bool isLoad(const char *name);

	/* Set a page on viewing */
	void setPageOnView(uint16_t NovelID, uint16_t PageNumber);

	/* Select a novel from novel stack */
	NovelInfo_t Select(const char *name);

private:
	/* Read whole content of a novel file */
	int Read(NovelInfo_t &novel);

	int HalfWordFilter(uint8_t *buf, uint16_t old_size);

	uint16_t DispSizeCalculator(uint8_t *src, uint16_t disp_max, uint16_t line_size_max);

	/* Add one page into page list */
	bool Add(const char *src, uint16_t count);

	/* get a file size */
	off_t getFileSize(const char *path);

	/* Login */
	bool Login(const char *url);

	/* Download a ebook to basic */
	NovelInfo_t Download(const char *name);

public:
	std::vector<PageInfo_t> m_PageCache; //!< operate instance(cache)

	std::vector<NovelInfo_t> m_NovelList; //!< operate instance

private:
	bool isFirst = true;
	uint16_t m_nCurrnetID = 0;
	uint16_t m_nCurrentPageNum = 0;
};

#endif
