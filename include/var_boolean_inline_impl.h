#pragma once


#include "var_boolean.h"
#include "var_number.h"
#include "var_string.h"
#include "var_integer.h"


namespace Cala {

    inline Boolean::Boolean(const bool & b) : m_bool(b) {}

    Boolean::operator bool() const {
        return m_bool;
    }

    bool Boolean::operator!() const {
        return !m_bool;
    }

    Boolean & Boolean::operator=(const bool & boolean) {
        m_bool = boolean;
        return *this;
    }


    ostream & operator<<(ostream & stream, const Boolean & self) {
        return stream << cast<String>(self);
    }

    template <typename TBoolean>
    bool Boolean::operator==(const TBoolean &other) const {
        return m_bool == static_cast<bool>(other);
    }

    template <typename TBoolean>
    bool Boolean::operator!=(const TBoolean &other) const {
        return m_bool != static_cast<bool>(other);
    }

    bool Boolean::toggle() {
        return (m_bool = !m_bool);
    }

    bool Boolean::value() const {
        return m_bool;
    }

    bool Boolean::operator*() const {
        return m_bool;
    }

    void Boolean::print(ostream &stream) {
        stream << *this;
    }



    Boolean Boolean::cast(const Null &){
        return false;
    }
    Boolean Boolean::cast(const Undefined &){
        return false;
    }
    Boolean Boolean::cast(const Number & number){
        return static_cast<bool>(+number);
    }
    Boolean Boolean::cast(const Integer & integer){
        return static_cast<bool>(+integer);
    }
    Boolean Boolean::cast(const String & string){
        return !string.isEmpty();
    }
    Boolean Boolean::cast(const List &){
        return true;
    }
    Boolean Boolean::cast(const Object &){
        return true;
    }
    Boolean Boolean::cast(const Function &){
        return true;
    }







}

#include "var_boolean_inline_impl.h"




