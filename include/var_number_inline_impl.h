#pragma once
#include "var_number.h"
#include "var_boolean.h"
#include "var_substring.h"

namespace Cala {

    inline Number operator""_n(long double number){
        return Number(static_cast<float_t>(number));
    }

    inline Number operator""_n(unsigned long long integer){
        return Number(static_cast<float_t>(integer));
    }

    Number::operator bool() const     { return static_cast<bool>(m_float); }
    Number::operator float_t() const  { return m_float; }
    float_t Number::value() const     { return m_float; }
    float_t Number::operator*() const { return m_float; }

    /* friend */ ostream & operator<<(ostream & stream, const Number & self) {
        return stream << self.m_float;
    }

    void Number::print(ostream &stream) {
        stream << *this;
    }

    bool Number::isFinite()   const { return std::isfinite(m_float);    }
    bool Number::isNegative() const { return m_float < 0;               }
    bool Number::isPositive() const { return m_float > 0;               }
    bool Number::isZero()     const { return *this == 0;                }

    bool Number::isNaN() const        { return std::isnan(m_float); }
    bool Number::isInfinity() const   { return std::isinf(m_float); }

    bool Number::operator !() const {
        return !static_cast<bool>(m_float);
    }

    Number Number::cast(const Boolean & boolean) noexcept {
        return static_cast<float_t>(*boolean);
    }

    Number Number::cast(const Null &)      noexcept {
        return 0.0_n;
    }
    Number Number::cast(const Undefined &) noexcept {
        return nan;
    }

    float_t Number::operator+() const {
        return m_float;
    }

    float_t Number::operator-() const {
        return -m_float;
    }

    Number && Number::operator~() && {
        m_float = ~static_cast<int_t>(m_float);
        return static_cast<Number &&>(*this);
    }

    Number Number::operator~() const & {
        return ~static_cast<int_t>(m_float);
    }

    Number Number::cast(const Substring & substr) noexcept {
        return Number::cast(substr.toString());
    }

    Number Number::parse(const Substring & substr) noexcept {
        return parse(substr.toString());
    }


}
