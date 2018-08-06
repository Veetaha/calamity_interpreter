//
// Created by tegeran on 07.07.18.
//

#ifndef CALAMITY_OUT_NATIVE_ERROR_H
#define CALAMITY_OUT_NATIVE_ERROR_H

#include "defs.h"
#include "var_string.h"

namespace Cala {

    class Exception {
    private:
        String m_errorString;

    public:
        inline explicit Exception(String && errorString)
            : m_errorString(std::move(errorString)) {
            Debug_log(ca("Cala::Exception instance was thrown -> ") << m_errorString);
        }

        inline const String & errorString() const noexcept {
            return m_errorString;
        }

        inline String & errorString() noexcept {
            return m_errorString;
        }


        inline const cachar_t * what() const noexcept {
            return m_errorString.c_str();
        }

        inline friend ostream & operator<<(ostream & stream, const Exception & self){
            return stream << self.m_errorString;
        }
    };

}




#endif //CALAMITY_OUT_NATIVE_ERROR_H
