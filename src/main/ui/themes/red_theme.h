#ifndef _RED_THEME_H_
#define _RED_THEME_H_

#include "theme.h"

namespace amts {
    using namespace omni::types;

    class RedTheme : public Theme {
        public:
            RedTheme();

            void apply_theme() override;
    };
    
}

#endif
