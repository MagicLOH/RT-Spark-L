#ifndef __NOVEL_PRESENTER_H
#define __NOVEL_PRESENTER_H

#include "ReaderView.h"
#include "ReaderModel.h"

namespace Page
{
    class Novel : public PageBase
    {
    public:
        typedef struct
        {
            uint16_t time;
            lv_color_t color;
        } Param_t;

    public:
        Novel();
        virtual ~Novel();

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
