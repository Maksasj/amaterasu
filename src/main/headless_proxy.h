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
    };
}

#endif