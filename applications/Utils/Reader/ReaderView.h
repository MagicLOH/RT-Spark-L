#ifndef __READER_VIEW_H
#define __READER_VIEW_H

#include "Page.h"

namespace Page
{

	class ReaderView
	{
	public:
		void Create(lv_obj_t *root);

	public:
		struct
		{
			lv_obj_t *labelTitle;
			lv_obj_t *labelTick;
		} ui;

	private:

	};

}

#endif /* READER_VIEW_H */
