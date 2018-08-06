#pragma once

#include <cstdint>
#include <iostream>
#include "defs.h"
#include "meta.h"
#include "vtem.h"

namespace Cala {

	template<typename TInt>
	class BasicInteger{
		TInt m_int;

	public:

	    typedef TInt wrapped_type;
		typedef TInt value_type;
		typedef TInt int_type;

		DECL_DEFAULT_COPY_AND_MOVE(BasicInteger)

		BasicInteger(const TInt &integer = 0) : m_int(integer) {}

		inline operator TInt() const          { return m_int; }
		inline TInt value() const             { return m_int; }
		inline TInt operator*() const         { return m_int; }

		explicit operator bool() const        { return static_cast<bool>(m_int); }

		inline static constexpr TInt max()    { return std::numeric_limits<TInt>::max(); }
		inline static constexpr TInt min()    { return std::numeric_limits<TInt>::min(); }

		bool isZero() const                   { return !m_int;    }
		bool isPositive() const               { return m_int > 0; }
		bool isNegative() const               { return m_int > 0; }

		inline static constexpr size_t size() { return sizeof(TInt); }

		static BasicInteger cast(const Number  & number);
		static BasicInteger cast(const Boolean & boolean);


		meta_inline BasicInteger && operator ~() && {
			m_int = ~m_int;
			return static_cast<BasicInteger &&>(*this);
		}
		meta_inline BasicInteger operator ~() const & { return ~m_int;  }
		meta_inline bool operator !() const           { return !m_int;  }
		meta_inline TInt operator+() const            { return  m_int;  }
		meta_inline TInt operator-() const            { return -m_int;  }


		#define Integer_def_postOperator(OPERATOR)                  \
		meta_inline const BasicInteger operator OPERATOR (int) & {\
            return m_int OPERATOR;                                  \
        }

		#define Integer_def_preOperator(OPERATOR)            \
		meta_inline BasicInteger & operator OPERATOR () & {\
			return (OPERATOR m_int), *this;                  \
		}

		#define Integer_def_prePostOperators(OPERATOR) \
		Integer_def_preOperator(OPERATOR)              \
		Integer_def_postOperator(OPERATOR)


		#define Integer_def_assignmentOperator(OPERATOR)                       \
		template <typename TNumeric>                                           \
		meta_inline BasicInteger & operator OPERATOR(const TNumeric & num) { \
			m_int OPERATOR static_cast<TInt>(num);                             \
			return *this;                                                      \
		}

		#define Integer_def_relationalOperator(OPERATOR, FUNC)             \
		template <typename TNumeric>                                       \
		meta_inline bool operator OPERATOR(const TNumeric & num) const { \
			return (FUNC)(m_int, num);                                     \
		}

		#define Integer_def_arithmeticOperator(OPERATOR)                                       \
		template <typename TNumeric>                                                           \
		meta_inline BasicInteger<TInt> && operator OPERATOR(const TNumeric & num) && {       \
			m_int OPERATOR##= static_cast<TInt>(num);                                          \
			return static_cast<BasicInteger<TInt> &&>(*this);                                  \
 	    }                                                                                      \
		template <typename TNumeric>                                                           \
		meta_inline BasicInteger<TInt> operator    OPERATOR(const TNumeric & num) const & {  \
			return m_int OPERATOR static_cast<TInt>(num);                                      \
		}

		Integer_def_prePostOperators(++)
		Integer_def_prePostOperators(--)

		Integer_def_assignmentOperator(=)
		Integer_def_assignmentOperator(+=)
		Integer_def_assignmentOperator(-=)
		Integer_def_assignmentOperator(*=)
		Integer_def_assignmentOperator(/=)
		Integer_def_assignmentOperator(%=)
		Integer_def_assignmentOperator(^=)
		Integer_def_assignmentOperator(|=)
		Integer_def_assignmentOperator(&=)


		Integer_def_relationalOperator(==, Vtem::isEqual   )
		Integer_def_relationalOperator(!=, Vtem::isNotEqual)
		Integer_def_relationalOperator(< , Vtem::isLess    )
		Integer_def_relationalOperator(<=, Vtem::isLeq     )
		Integer_def_relationalOperator(> , Vtem::isGreater )
		Integer_def_relationalOperator(>=, Vtem::isGeq     )

		Integer_def_arithmeticOperator(+)
		Integer_def_arithmeticOperator(-)
		Integer_def_arithmeticOperator(*)
		Integer_def_arithmeticOperator(/)
		Integer_def_arithmeticOperator(%)
		Integer_def_arithmeticOperator(^)
		Integer_def_arithmeticOperator(|)
		Integer_def_arithmeticOperator(&)
	};

	list_iterator operator+(const list_iterator & iterator, const Integer & adding);
	list_iterator operator-(const list_iterator & iterator, const Integer & subtracting);
    list_const_iterator operator+(const list_const_iterator & iterator, const Integer & adding);
    list_const_iterator operator-(const list_const_iterator & iterator, const Integer & subtracting);

    string_iterator operator+(const string_iterator & iterator, const Integer & adding);
    string_iterator operator-(const string_iterator & iterator, const Integer & subtracting);
    string_const_iterator operator+(const string_const_iterator & iterator, const Integer & adding);
    string_const_iterator operator-(const string_const_iterator & iterator, const Integer & subtracting);

}

