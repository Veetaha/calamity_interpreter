//
// Created by tegeran on 07.07.18.
//

#include "var_substring.h"

namespace Calamity {

    std::wostream & operator<<(std::wostream & stream, const Substring & self){
        for(const auto & character : self){
            stream << static_cast<wchar_t>(character);
        }
        return stream;
    }



}