#pragma once

#include <cmath>
#include <iostream>
#include "std_ext.h"
#include "var_boolean.h"
#include "var_integer.h"
#include <boost/lexical_cast.hpp>

namespace Calamity {

	class Double {
		double m_double;
	public:
		DECL_DEFAULT_COPY_AND_MOVE(Double);

		Double(const double &d = 0) : m_double(d) {}

		static const Double & nan();
		static const Double & infinity();

		inline explicit operator double() const { return m_double; }
		inline double value() const             { return m_double; }

		inline friend std::wostream &operator<<(std::wostream &stream, const Double &self)
		{ return stream << self.m_double; }

		inline void print(std::wostream &stream = std::wcout)
		{ stream << *this; }

		inline bool isNaN() const        { return std::isnan(m_double);                  }
		inline bool isInfinity() const   { return std::isinf(m_double);                  }
		inline String toString() const   {
		    return boost::lexical_cast<String::string_t>(m_double);
		}

		inline bool to_bool() const      { return *this != 0; }
		inline operator bool() const     { return *this != 0; }
		inline Boolean toBoolean() const { return *this != 0; }

		inline int64_t to_int64() const      { return static_cast<int64_t >(m_double); }
		inline operator int64_t() const      { return static_cast<int64_t >(m_double); }
		inline Integer64 toInteger64() const { return static_cast<int64_t >(m_double); }


		static inline constexpr size_t size() { return sizeof(m_double); }
		static inline constexpr double max()  { return std::numeric_limits<double>::max(); }
		static inline constexpr double min()  { return std::numeric_limits<double>::min(); }

		inline bool isNegative() const { return m_double < 0; }
		inline bool isPositive() const { return m_double > 0; }
		inline bool isZero() const     { return *this == 0;   }

		inline bool operator !() const { return !to_bool(); }

		inline Double && operator ~() && {
			m_double = ~static_cast<int64_t>(m_double);
			return static_cast<Double &&>(*this);
		}

		inline Double operator ~() const & {
			return ~static_cast<int64_t>(m_double);
		}

		template<typename TNumeric>
		inline Double & pow(const TNumeric &num){
			m_double = std::pow(m_double, static_cast<double>(num));
			return *this;
		}

		template<typename TNumeric>
		inline Double & abs(const TNumeric &num){
			m_double = std::abs(static_cast<double>(num));
			return *this;
		}


		template<typename TNumeric>
		inline Double &&operator%(const TNumeric &num) &&{
			m_double = std::remainder(m_double, static_cast<double>(num));
			return static_cast<Double &&>(*this);
		}

		template<typename TNumeric>
		inline Double operator%(const TNumeric &num) &{
			return std::remainder(m_double, static_cast<double>(num));
		}

		template<typename TNumeric>
		inline Double &operator%=(const TNumeric &num) {
			m_double = std::remainder(m_double, static_cast<double>(num));
			return *this;
		}

		// RELATIONS ----------------------------------------------------------

		template<typename TNumeric>
		inline bool operator==(const TNumeric &other) const {
			return std_ext::isEqual(m_double, static_cast<double>(other));
		}

		template<typename TNumeric>
		inline bool operator!=(const TNumeric &other) const {
			return !(operator==(other));
		}


		#define Double_def_relationalOperator(OPERATOR)            \
		template<typename TNumeric>							       \
		inline bool operator OPERATOR(const TNumeric &num) const { \
			return m_double OPERATOR num;                          \
		}

		#define Double_def_assignmentOperator(OPERATOR)           \
		template<typename TNumeric>                               \
		inline Double & operator OPERATOR(const TNumeric &num) {  \
			m_double OPERATOR static_cast<double>(num);           \
			return *this;                                         \
		}

		#define Double_def_bitwiseAssignmentOperator(BITWISE_OPERATOR)                    \
		template<typename TNumeric>                                                       \
		inline Double & operator BITWISE_OPERATOR##=(const TNumeric &num) {               \
			m_double = static_cast<double>(                                               \
				static_cast<int64_t>(m_double) BITWISE_OPERATOR static_cast<int64_t>(num) \
			);                                                                            \
			return *this;                                                                 \
		}

		#define Double_def_arithmeticOperator(OPERATOR)                 \
		template<typename TNumeric>                                     \
		inline Double && operator OPERATOR(const TNumeric &num) && {    \
			m_double OPERATOR##= static_cast<double>(num);              \
			return static_cast<Double &&>(*this);                       \
		}     												            \
		template<typename TNumeric>                                     \
		inline Double operator OPERATOR(const TNumeric &num) const & {  \
			return m_double OPERATOR static_cast<double>(num);          \
		}

		#define Double_def_bitwiseOperator(OPERATOR)                               \
		template<typename TNumeric>                                                \
		inline Double && operator OPERATOR(const TNumeric &num) && {               \
			m_double = static_cast<double>(                                        \
				static_cast<int64_t>(m_double) OPERATOR static_cast<int64_t>(num)  \
			);                                                                     \
			return static_cast<Double &&>(*this);                                  \
		}     												                       \
		template<typename TNumeric>                                                \
		inline Double operator OPERATOR(const TNumeric &num) const & {             \
			return static_cast<double>(                                            \
				static_cast<int64_t>(m_double) OPERATOR static_cast<int64_t>(num)  \
			);                                                                     \
		}


		Double_def_relationalOperator(<)
		Double_def_relationalOperator(<=)
		Double_def_relationalOperator(>)
		Double_def_relationalOperator(>=)


		Double_def_assignmentOperator(=)
		Double_def_assignmentOperator(+=)
		Double_def_assignmentOperator(-=)
		Double_def_assignmentOperator(*=)
		Double_def_assignmentOperator(/=)
		Double_def_bitwiseAssignmentOperator(^)
		Double_def_bitwiseAssignmentOperator(&)
		Double_def_bitwiseAssignmentOperator(|)
        Double_def_bitwiseAssignmentOperator(<<)
        Double_def_bitwiseAssignmentOperator(>>)

		Double_def_arithmeticOperator(+)
		Double_def_arithmeticOperator(-)
		Double_def_arithmeticOperator(*)
		Double_def_arithmeticOperator(/)


		Double_def_bitwiseOperator(^)
		Double_def_bitwiseOperator(&)
		Double_def_bitwiseOperator(|)
        Double_def_bitwiseOperator(<<)
        Double_def_bitwiseOperator(>>)

	};

}