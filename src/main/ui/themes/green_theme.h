#ifndef _GREEN_THEME_H_
#define _GREEN_THEME_H_

#include "theme.h"

namespace amts {
    using namespace omni::types;

    class GreenTheme : public Theme {
        public:
            GreenTheme();

            void apply_theme() override;
    };
    
}

#endif
