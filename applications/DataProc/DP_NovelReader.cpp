#include "DataProc.h"
#include "HAL.h"

using namespace DataProc;

static void onNotify(Account *account, NovelReader_Info_t *info)
{
	switch (info->cmd)
	{
		case NOVEL_READER_CMD_SHOW:
		{
			RT_ASSERT(info->NovelContext);
			HAL::LCD_ShowString(0,
			                    CHN_FONT_24x24,
			                    CHN_FONT_24x24,
			                    "%s",
			                    info->NovelContext);
			delete info->NovelContext;
		}
			break;

		case NOVEL_READER_CMD_EXIT:
			break;
		case NOVEL_READER_CMD_BACK:
			break;
	}
}

static int onEvent(Account *account, Account::EventParam_t *param)
{
	if (param->event != Account::EVENT_NOTIFY)
	{
		return Account::RES_UNSUPPORTED_REQUEST;
	}

	if (param->size != sizeof(DataProc::NovelReader_Info_t))
	{
		return Account::RES_SIZE_MISMATCH;
	}

	DataProc::NovelReader_Info_t *info = (DataProc::NovelReader_Info_t *)param->data_p;
	onNotify(account, info);

	return Account::RES_OK;
}

DATA_PROC_INIT_DEF(NovelReader)
{
	account->SetEventCallback(onEvent);
}
