#ifndef _SIMPLE_THEME_H_
#define _SIMPLE_THEME_H_

#include "theme.h"

namespace amts {
    using namespace omni::types;

    class SimpleTheme : public Theme {
        public:
            SimpleTheme();

            void apply_theme() override;
    };
    
}

#endif
