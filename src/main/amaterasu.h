#ifndef _AMATERASU_H_
#define _AMATERASU_H_

#include "ui_proxy.h"
#include "headless_proxy.h"

namespace amts {
    class Amaterasu {
        private:
            std::unique_ptr<CommonProxy> m_proxy;

        public:
            Amaterasu() {
                m_proxy = std::make_unique<UIProxy>();
            }

            ~Amaterasu() {
                m_proxy = nullptr;
            }

            void preinit() {
                m_proxy->preinit();
            }

            void init() {
                m_proxy->init();
            }
            
            void load() {
                m_proxy->load();
            }

            void run() {
                m_proxy->run();
            }

            void unload() {
                m_proxy->unload();
            }

            void cleanup() {
                m_proxy->cleanup();
            }
    };
}

#endif