#ifndef _DRACULA_THEME_H_
#define _DRACULA_THEME_H_

#include "theme.h"

namespace amts {
    using namespace omni::types;

    class DraculaTheme : public Theme {
        public:
            DraculaTheme();

            void apply_theme() override;
    };
    
}

#endif
