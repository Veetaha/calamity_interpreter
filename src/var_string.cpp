//
// Created by tegeran on 07.07.18.
//

#include "var_string.h"

namespace Calamity {
    template<>
    BasicString<char16_t> BasicString<char16_t>::readFromFile(const char * const & path) {
        using namespace std::literals::string_literals;
        std::basic_ifstream<char16_t> file(path);
        if (! file.is_open()) {
            throw MessageException("failed to open file \""s + path + "\"");
        } else {
            std::basic_ostringstream<char16_t> contentHolder;
            contentHolder << file.rdbuf();
            return contentHolder.str();
        }
    }

    std::wostream & operator<<(std::wostream & stream, const String & self){
        return stream << self.c_str();
    }

    std::wostream & operator<<(std::wostream & stream, const char16_t * u16CString){
        while (*u16CString){
            stream << static_cast<wchar_t>(*u16CString++);
        }
        return stream;
    }
}