#ifndef _THEME_H_
#define _THEME_H_

#include <string>
#include "common/common.h"

namespace amts {
    class Theme {
        private:
            std::string _name;
            std::string _author;
            
        public:
            Theme(const std::string& name, const std::string& author) : _name(name), _author(author) {

            }

            virtual ~Theme() {
                
            }

            std::string get_name() const {
                return _name;
            }
    
            std::string get_author() const {
                return _author;
            }

            virtual void apply_theme() {

            }
    };
    
}

#endif
