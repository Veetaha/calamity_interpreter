#pragma once

#include <cmath>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "defs.h"
#include "meta.h"

namespace Cala {
	class Number {
		float_t m_float;
	public:
		typedef float_t wrapped_type;
		typedef float_t value_type;


        Number(const Number & lval) = default;
        Number(Number && rval) = default;
		Number() = default;
		Number(const float_t &d) : m_float(d) {} // implicit
		Number(float_t && d)     : m_float(d) {} // implicit
		Number(const size_t &d)  : m_float(d) {} // implicit
		Number(size_t && d)      : m_float(d) {} // implicit
        Number(const int &d)     : m_float(d) {} // implicit
        Number(int && d)         : m_float(d) {} // implicit
        Number(const int_t &d)   : m_float(d) {} // implicit
        Number(int_t && d)       : m_float(d) {} // implicit

		friend inline Number operator""_n(long double number);
		friend inline Number operator""_n(unsigned long long integer);

		/**
		 * @return Quiet not-a-number value
		 */
		static const Number nan;
		/**
		 * @return Positive infinity value
		 */
		static const Number infinity;

		static constexpr const size_t size = sizeof(float_t);
		static const Number max;
		static const Number min;

        /**
         * @brief Tries to parse first floating point number met in the string.
         * @param string    A string to parse.
         * @return  A number parsed in a stringif success, otherwise NaN.
         *
         * Uses std::stod in order to parse the string.
         * See https://en.cppreference.com/w/cpp/string/basic_string/stof in oreder
         * to learn which patterns it recognizes
         *
         * In case of an std::invalid_argument exception returns Number::nan() value
         * In case of an std::out_of_range exception returns Number::infinity() with the
         * proper sign
         *
         */
		static Number parse(const String & string) noexcept;

		/**
		 *  @brief The same as parse(const String &), but for Substring
		 */
        meta_inline static Number parse(const Substring & substr) noexcept;

        /**
         * @pre
         * @return
         */
        static Number parseBeginTrimmed(const String & string);
	    static Number parseBeginTrimmed(const Substring & substr);


		static Number cast(const String & string)    noexcept;
        static meta_inline Number cast(const Substring & substr) noexcept;
        static meta_inline Number cast(const Null &)             noexcept;
        static meta_inline Number cast(const Undefined &)        noexcept;
        static meta_inline Number cast(const Boolean & boolean)  noexcept;

        meta_inline explicit operator bool() const;
		meta_inline explicit operator float_t() const;

        /**
         * @return Primitive float_t value of wrapped by Number.
         *
         * The same as Number::operator*() and Number::operator+() (unary)
         */
		meta_inline float_t value() const;
		/**
		 * @return Primitive float_t value of wrapped by Number.
		 *
		 * The same as Number::value() and Number::operator+() (unary)
		 */
		meta_inline float_t operator*() const;

		meta_inline friend ostream &operator<<(ostream &stream, const Number &self);

		meta_inline void print(ostream &stream = conout);

		meta_inline bool isNaN() const;
		meta_inline bool isInfinity() const;




		meta_inline bool isFinite()   const;
		meta_inline bool isNegative() const;
		meta_inline bool isPositive() const;
		meta_inline bool isZero()     const;

		meta_inline bool      operator!() const;
		meta_inline float_t   operator+() const;
		meta_inline float_t   operator-() const;
		meta_inline Number && operator~() &&;
		meta_inline Number    operator~() const &;

		template <typename TNumeric>
		meta_inline Number pow(const TNumeric & power) const &{
			return std::pow(m_float, static_cast<float_t>(power));
		}

        template <typename TNumeric>
        meta_inline Number pow(const TNumeric & power) && {
            return std::pow(m_float, static_cast<float_t>(power));
        }

        meta_inline Number pow(Number && power) const & {
            power.m_float = std::pow(m_float, power.m_float);
            return static_cast<Number &&>(power);
        }


		meta_inline Number abs() const &{
			return std::abs(m_float);
		}

		meta_inline Number && abs() && {
		    return (m_float = std::abs(m_float)), static_cast<Number &&>(*this);
		}

		// OPERATORS ----------------------------------------------------------


		#define Number_apply_bitwiseOperator(LHS, OPERATOR, RHS)\
		(static_cast<int_t>((LHS)) OPERATOR (static_cast<int_t>(RHS)))

		#define Number_def_bitwiseOperator(OPERATOR)          \
		Meta_wrapper_def_arithmeticAndAssignmentOperators(Number, m_float, OPERATOR, Number_apply_bitwiseOperator)

		#define Number_def_arithmeticOperator(OPERATOR)                             \
		Meta_wrapper_def_arithmeticAndAssignmentOperators(Number, m_float, OPERATOR)\

		#define Number_def_relationalOperator(OPERATOR)\
		Meta_wrapper_def_relationalOperator(Number, m_float, OPERATOR)

		#define Number_def_prePostOperators(OPERATOR)\
		Meta_wrapper_def_prePostOperators(Number, m_float, OPERATOR)

        #define Number_applyRemainder(LHS, OPERATOR, RHS) std::remainder((LHS), (RHS))
		Meta_wrapper_def_arithmeticAndAssignmentOperators(
			Number,
			m_float,
			%,
			Number_applyRemainder
		)
        // trivial assignment operator:
        Meta_wrapper_def_assignmentOperator(Number, m_float)

		Number_def_prePostOperators(++)
		Number_def_prePostOperators(--)
        Number_def_relationalOperator(==)
        Number_def_relationalOperator(!=)
		Number_def_relationalOperator(<)
		Number_def_relationalOperator(<=)
		Number_def_relationalOperator(>)
		Number_def_relationalOperator(>=)
		// compound assignment is included
		Number_def_arithmeticOperator(+)
		Number_def_arithmeticOperator(-)
        Number_def_arithmeticOperator(*)
        Number_def_arithmeticOperator(/)
		// compound assignment is included
		Number_def_bitwiseOperator(^)
		Number_def_bitwiseOperator(&)
		Number_def_bitwiseOperator(|)
        Number_def_bitwiseOperator(<<)
        Number_def_bitwiseOperator(>>)

	};

	inline constexpr const Number & Infinity = Number::infinity;
	inline constexpr const Number & NaN      = Number::nan;

}




#include "var_number_inline_impl.h"