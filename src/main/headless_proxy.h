#ifndef _HEADLESS_PROXY_H_
#define _HEADLESS_PROXY_H_

#include "common/common_proxy.h"

namespace amts {
    class HeadlessProxy : public CommonProxy {
        private:

        public:
            HeadlessProxy() : CommonProxy() { 

            }

            ~HeadlessProxy() override {
                
            }

            void preinit() override {
                CommonProxy::preinit();
            }

            void init() override {
                CommonProxy::init();
            }

            void load() override {
                CommonProxy::load();
            }

            void run() override {
                CommonProxy::run();
            }

            void unload() override {
                CommonProxy::unload();
            }

            void cleanup() override {
                CommonProxy::cleanup();
            }
    };
}

#endif