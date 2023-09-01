#ifndef __DATA_PROC_DEF_H
#define __DATA_PROC_DEF_H

#include <stdint.h>

namespace DataProc
{
	/* StatusBar */
	typedef enum
	{
		STATUS_BAR_STYLE_TRANSP,
		STATUS_BAR_STYLE_BLACK,
	} StatusBar_Style_t;

	typedef enum
	{
		STATUS_BAR_CMD_APPEAR,
		STATUS_BAR_CMD_SET_STYLE,
		STATUS_BAR_CMD_SET_LABEL_REC
	} StatusBar_Cmd_t;

	typedef struct
	{
		StatusBar_Cmd_t cmd;
		union
		{
			bool appear;
			StatusBar_Style_t style;
			struct
			{
				bool show;
				const char *str;
			} labelRec;
		} param;
	} StatusBar_Info_t;

	/* SysConfig */
	typedef enum
	{
		SYSCONFIG_CMD_LOAD,
		SYSCONFIG_CMD_SAVE,
	} SysConfig_Cmd_t;

	typedef struct
	{
		SysConfig_Cmd_t cmd;
		float longitude;
		float latitude;
		int16_t timeZone;
		bool soundEnable;
		char language[8];
		char arrowTheme[16];
		char mapDirPath[16];
		char mapExtName[8];
		bool mapWGS84;
	} SysConfig_Info_t;

	/* NovelReader */
	typedef enum
	{
		NOVEL_READER_CMD_SHOW, // show string
		NOVEL_READER_CMD_EXIT, // exit to main menu
		NOVEL_READER_CMD_BACK, // back to bookshelf
	} NovelReader_Cmd_t;

	typedef struct
	{
		NovelReader_Cmd_t cmd;
		const char *NovelName;
		char *NovelContext;
	} NovelReader_Info_t;

}

#endif
