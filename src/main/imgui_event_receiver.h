#ifndef _IMGUI_EVENT_RECEIVER_H_
#define _IMGUI_EVENT_RECEIVER_H_

#include "common/common.h"
#include "event_receiver.h"

namespace amts {
    class ImGuiEventReceiver : public EventReceiver {
        private:

        public:
            void handle_event(const SDL_Event& event) override {
                ImGui_ImplSDL3_ProcessEvent(&event);
            }
    };
}

#endif
