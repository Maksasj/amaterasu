#ifndef _EVENT_HANDLER_H_
#define _EVENT_HANDLER_H_

#include <vector>
#include "common/common.h"
#include "event_receiver.h"

namespace amts {
    class EventHandler {
        private:
            std::vector<EventReceiver*> m_eventReceivers;

        public:
            void handle_events() {
                SDL_Event event;

                while (SDL_PollEvent(&event)) {
                    for(EventReceiver* eventReceivere : m_eventReceivers)
                        eventReceivere->handle_event(event);
                }
            }

            void bind_event_receiver(EventReceiver* eventReceiver) {
                m_eventReceivers.push_back(eventReceiver);
            }
    };
}

#endif