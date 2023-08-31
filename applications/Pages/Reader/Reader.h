#ifndef __READER_PRESENTER_H
#define __READER_PRESENTER_H

#include "ReaderView.h"
#include "ReaderModel.h"

#ifdef PKG_USING_LVGL

namespace Page
{
	class Reader : public PageBase
	{
	public:
		typedef struct
		{
			uint16_t time;
			lv_color_t color;
		} Param_t;

	public:
		Reader();
		virtual ~Reader();

		virtual void onCustomAttrConfig();
		virtual void onViewLoad();
		virtual void onViewDidLoad();
		virtual void onViewWillAppear();
		virtual void onViewDidAppear();
		virtual void onViewWillDisappear();
		virtual void onViewDidDisappear();
		virtual void onViewUnload();
		virtual void onViewDidUnload();

	private:
		void Update();
		void AttachEvent(lv_obj_t *obj);
		static void onTimerUpdate(lv_timer_t *timer);
		static void onEvent(lv_event_t *event);

	private:
		ReaderView View;
		ReaderModel Model;
		lv_timer_t *timer;
	};

}

#endif

#endif
