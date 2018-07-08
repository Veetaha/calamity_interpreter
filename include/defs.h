#ifndef TASK3_OUT_DEFS_H
#define TASK3_OUT_DEFS_H

#include <iostream>
#include <locale>
#include <fstream>
#include "meta.h"
#include "error.h"

namespace Calamity {
#define ca(STRING_OR_CHAR_LITERAL) Macro_concatenate(L, STRING_OR_CHAR_LITERAL)
    using cachar_t = wchar_t;
    inline constexpr const cachar_t endline = ca('\n');

    using ios           = std::basic_ios<cachar_t>;
    using streambuf     = std::basic_streambuf<cachar_t>;
    using istream       = std::basic_istream<cachar_t>;
    using ostream       = std::basic_ostream<cachar_t>;
    using iostream      = std::basic_iostream<cachar_t>;
    using stringbuf     = std::basic_stringbuf<cachar_t>;
    using istringstream = std::basic_istringstream<cachar_t>;
    using ostringstream = std::basic_ostringstream<cachar_t>;
    using stringstream  = std::basic_stringstream<cachar_t>;
    using filebuf       = std::basic_filebuf<cachar_t>;
    using ifstream      = std::basic_ifstream<cachar_t>;
    using ofstream      = std::basic_ofstream<cachar_t>;
    using fstream       = std::basic_fstream<cachar_t>;
    inline ostream & conout = std::wcout;

    inline constexpr const auto _B_E_F_O_R_E___M_A_I_N_ = [](){
        std::locale::global(std::locale(""));
    };


    inline ostream & operator<<(ostream & stream, const char * cString){
        while (*cString) stream << static_cast<cachar_t>(*cString++);
        return stream;
    }



#define CaDebug_log(MESSAGE...)                \
(::Calamity::conout << ca("CaDebug_log at ")   \
                     << __func__               \
                     << ca(" -> ")             \
                     << MESSAGE                \
                     << ca('\n'))
#define CaDebug_logError(MESSAGE...)                 \
(::Calamity::conout << ca("************* error at ") \
                    << __func__                      \
                    << " -> "                        \
                    << MESSAGE                       \
                    << ca('\n'))
#define CaDebug_logIf(CONDITION, MESSAGE) ({                                \
if ((CONDITION)){                                                           \
    (::Calamity::conout << ca("CaDebug_logIf at ")                          \
                        << __func__                                         \
                        << ca(" -> ")                                       \
                        << MESSAGE                                          \
                        << ca(" | triggered by condition ")                 \
                        << Macro_concatenate(L, Macro_stringify(CONDITION));\
    }                                                                       \
})

#define CaDebug_suppose(SUPPOSITION, ERROR_MESSAGE...) Debug_suppose(SUPPOSITION, ##ERROR_MESSAGE)
#define CaDebug_noimpl() Debug_shutdown(ca("program workflow has reached unimplemented line of code"))
#define CaDebug_unreachable() Debug_shutdown(ca("program workflow has reached an unreachable line of code"))


}

#endif //TASK3_OUT_DEFS_H


