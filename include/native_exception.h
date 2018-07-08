//
// Created by tegeran on 07.07.18.
//

#ifndef CALAMITY_OUT_NATIVE_ERROR_H
#define CALAMITY_OUT_NATIVE_ERROR_H

#include "defs.h"
#include "var_string.h"

namespace Calamity {

    class Exception {
    private:
        String m_errorString;

    public:
        inline explicit Exception(String && errorString)
            : m_errorString(std::move(errorString)) {
            CaDebug_log(ca("Calamity::Exception instance was thrown -> ") << m_errorString.c_str());
        }
        inline const cachar_t * what() const noexcept {
            return m_errorString.c_str();
        }

    };

};




#endif //CALAMITY_OUT_NATIVE_ERROR_H
