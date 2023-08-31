#include "DataProc.h"
#include "HAL.h"

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

	return Account::RES_OK;
}

DATA_PROC_INIT_DEF(NovelReader)
{
	account->SetEventCallback(onEvent);
}
