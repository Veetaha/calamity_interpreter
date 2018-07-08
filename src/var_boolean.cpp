#include "var_boolean.h"
#include <error.h>

namespace Calamity {

    Boolean::Boolean(const bool & b) : m_bool(b) {}

    Boolean::operator bool() const {
        return m_bool;
    }

    Boolean & Boolean::operator=(const bool & boolean) {
        m_bool = boolean;
        return *this;
    }


    std::wostream & operator<<(std::wostream & stream, const Boolean & self) {
        return stream << self.toWCString();
    }

    bool Boolean::operator==(const Boolean & other) const {
        return other.m_bool == m_bool;
    }

    bool Boolean::operator!=(const Boolean & other) const {
        return other.m_bool != m_bool;
    }

    bool Boolean::toggle() {
        return (m_bool = !m_bool);
    }

    bool Boolean::value() const {
        return m_bool;
    }

    const char * Boolean::toCString() const {
        return m_bool ? "true" : "false";
    }

    const char16_t * Boolean::toU16CString() const {
        return m_bool ? u"true" : u"false";
    }

    const wchar_t * Boolean::toWCString() const {
        return m_bool ? L"true" : L"false";
    }

    const cachar_t * Boolean::toCaString() const{
        return m_bool ? ca("true") : ca("false");
    }

    const String & Boolean::toString() const {
        static String trueString  = ca("true");
        static String falseString = ca("false");
        return m_bool ? trueString : falseString;
    }

}