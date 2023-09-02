#ifndef _EVENT_RECEIVER_H_
#define _EVENT_RECEIVER_H_

#include "common/common.h"

namespace amts {
    class EventReceiver {
        private:

        public:
            virtual ~EventReceiver() {
                
            }

            virtual void handle_event(const SDL_Event& event) = 0;
    };
}

#endif