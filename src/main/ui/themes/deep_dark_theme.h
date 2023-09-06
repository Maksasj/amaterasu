#ifndef _DEEP_DARK_THEME_H_
#define _DEEP_DARK_THEME_H_

#include "theme.h"

namespace amts {
    using namespace omni::types;

    class DeepDarkTheme : public Theme {
        public:
            DeepDarkTheme();

            void apply_theme() override;
    };
    
}

#endif
