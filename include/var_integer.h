//
// Created by tegeran on 28.06.18.
//

#ifndef TASK3_TEST_OUT_INTEGER64_H
#define TASK3_TEST_OUT_INTEGER64_H

#include <cstdint>
#include <iostream>
#include "std_ext.h"
#include "var_boolean.h"
#include "var_string.h"

namespace Calamity {

	template<typename TInt>
	class BasicInteger{
		TInt m_int;

	public:
		DECL_DEFAULT_COPY_AND_MOVE(BasicInteger);

		BasicInteger(const TInt &integer = 0) : m_int(integer) {}

		inline explicit operator TInt() const { return m_int; }
		inline TInt value() const             { return m_int; }

		inline String toString() const        { return std::to_string(m_int); }

		explicit operator bool() const        { return m_int; }
		bool to_bool() const                  { return m_int; }
		Boolean toBoolean() const             { return m_int; }

		inline static constexpr TInt max()    { return std::numeric_limits<TInt>::max(); }
		inline static constexpr TInt min()    { return std::numeric_limits<TInt>::min(); }

		bool isZero() const                   { return !m_int;    }
		bool isPositive() const               { return m_int > 0; }
		bool isNegative() const               { return m_int > 0; }

		inline static constexpr size_t size() { return sizeof(TInt); }

		template<typename TNumeric>
		inline BasicInteger && operator ~() && {
			m_int = ~m_int;
			return static_cast<BasicInteger &&>(*this);
		}

		template<typename TNumeric>
		inline BasicInteger operator ~() const & { return ~ m_int; }
		inline bool operator !() const      { return !m_int;  }

		#define Integer_def_assignmentOperator(OPERATOR)           \
		template <typename TNumeric>                               \
		inline BasicInteger & operator OPERATOR(const TNumeric & num) { \
			m_int OPERATOR static_cast<TInt>(num);                 \
			return *this;                                          \
		}

		#define Integer_def_relationalOperator(OPERATOR, FUNC)      \
		template <typename TNumeric>                                \
		inline bool operator OPERATOR(const TNumeric & num) const { \
			return (FUNC)(m_int, num);                              \
		}

		#define Integer_def_arithmeticOperator(OPERATOR)                           \
		template <typename TNumeric>                                               \
		inline BasicInteger<TInt> && operator OPERATOR(const TNumeric & num) && {       \
			m_int OPERATOR##= static_cast<TInt>(num);                              \
			return static_cast<BasicInteger<TInt> &&>(*this);                           \
 	    }                                                                          \
		template <typename TNumeric>                                               \
		inline BasicInteger<TInt> operator    OPERATOR(const TNumeric & num) const & {  \
			return m_int OPERATOR static_cast<TInt>(num);                          \
		}

		Integer_def_assignmentOperator(=)
		Integer_def_assignmentOperator(+=)
		Integer_def_assignmentOperator(-=)
		Integer_def_assignmentOperator(*=)
		Integer_def_assignmentOperator(/=)
		Integer_def_assignmentOperator(%=)
		Integer_def_assignmentOperator(^=)
		Integer_def_assignmentOperator(|=)
		Integer_def_assignmentOperator(&=)


		Integer_def_relationalOperator(==, std_ext::isEqual   )
		Integer_def_relationalOperator(!=, std_ext::isNotEqual)
		Integer_def_relationalOperator(< , std_ext::isLess    )
		Integer_def_relationalOperator(<=, std_ext::isLeq     )
		Integer_def_relationalOperator(> , std_ext::isGreater )
		Integer_def_relationalOperator(>=, std_ext::isGeq     )

		Integer_def_arithmeticOperator(+)
		Integer_def_arithmeticOperator(-)
		Integer_def_arithmeticOperator(*)
		Integer_def_arithmeticOperator(/)
		Integer_def_arithmeticOperator(%)
		Integer_def_arithmeticOperator(^)
		Integer_def_arithmeticOperator(|)
		Integer_def_arithmeticOperator(&)
	};

	typedef BasicInteger<int64_t> Integer64;

}

#endif //TASK3_TEST_OUT_INTEGER64_H
