//
// Created by tegeran on 27.06.18.
//

#ifndef TASK3_TEST_OUT_VAR_LIST_H
#define TASK3_TEST_OUT_VAR_LIST_H

#include <vector>
#include <iostream>
#include "defs.h"
#include "var_boolean.h"
#include "var_number.h"
#include "native_exception.h"

namespace Cala {

	class List {
		list_base m_vector;
	public:
        using size_type              = list_base::size_type;
		using iterator               = list_base::iterator;
		using const_iterator         = list_base::const_iterator;
		using value_type             = list_base::value_type;
		using reference              = list_base::reference;
		using const_reference        = list_base::const_reference;
		using reverse_iterator       = list_base::reverse_iterator;
		using const_reverse_iterator = list_base::const_reverse_iterator;


		inline iterator begin()                 { return m_vector.begin();   }
		inline iterator end()                   { return m_vector.end();     }
		inline const_iterator cbegin()          { return m_vector.cbegin();  }
		inline const_iterator cend()            { return m_vector.cend();    }
		inline reverse_iterator rbegin()        { return m_vector.rbegin();  }
		inline reverse_iterator rend()          { return m_vector.rend();    }
		inline const_reverse_iterator crbegin() { return m_vector.crbegin(); }
		inline const_reverse_iterator crend()   { return m_vector.crend();   }

		inline const_iterator begin()           const { return m_vector.begin();   }
		inline const_iterator end()             const { return m_vector.end();     }
		inline const_iterator cbegin()          const { return m_vector.cbegin();  }
		inline const_iterator cend()            const { return m_vector.cend();    }
		inline const_reverse_iterator rbegin()  const { return m_vector.rbegin();  }
		inline const_reverse_iterator rend()    const { return m_vector.rend();    }
		inline const_reverse_iterator crbegin() const { return m_vector.crbegin(); }
		inline const_reverse_iterator crend()   const { return m_vector.crend();   }

		int64_t size() const;
		inline bool isEmpty() const { return m_vector.empty(); }

		List() = default;
		List(const List & lvalue);
		List(List && rvalue) noexcept;

		List & operator=(const List & lvalue);
		List & operator=(List && rvalue) noexcept;

		List & operator+=(const Var & lvalue);
		List & operator+=(Var && rvalue);

		friend List operator+(const List & leftLval,  const List & rightLval);
		friend List operator+(List &&      leftRval,       List && rightRval);
		friend List operator+(List &&      leftRval,  const List & rightLval);
		friend List operator+(const List & leftLval,       List && rightRval);

        inline void reserve(const size_type & reservationSize)
        { m_vector.reserve(reservationSize); }

        inline void reserveMore(const size_type & more)
        { m_vector.reserve(m_vector.size() + more); }

		List & push_back(const List & lvalue);
		List & emplace_back(List &&rvalue);

		List & push_front(const List & lvalue);
		List & emplace_front(List && rvalue);

		List & push_back(const Var & lvalue);
		List & emplace_back(Var && rvalue);

		List & push_front(const Var & lvalue);
		List & emplace_front(Var && rvalue);

		explicit operator bool() const;

		Var & operator[](const Var & index) &;
		Var && operator[](const Var & index) &&;
		inline Var & front()             { return m_vector.front(); }
		inline const Var & front() const { return m_vector.front(); }
        inline Var & back()              { return m_vector.back();  }
        inline const Var & back() const  { return m_vector.back();  }

		void insert(const Var & index, Var && rvalue);
		void insert(const Var & index, const Var & lvalue);
		auto remove(const Var & index);

		List sublist(const Var & begin, const Var & end) &;
		List sublist(const Var & begin, const Var & end) &&;

		friend ostream & operator<<(ostream & stream, const List & self);

		void print(ostream & stream = conout)
		{ stream << *this; }


	private:
		void checkReadIndex(const Var & index) const; // throws MessageException
		void checkInsertIndex(const Var & index) const; // throws MessageException
		Exception negativeIndexException(const Number & index) const;
		Exception indexOutOfBoundsException(const Number & index) const;
		Exception nonNumericListSubscriptException(const Var &subscript) const;
		Exception sublistInvalidRangeTypeException(const Var &begin, const Var &end);
		Exception sublistRidiculousRangeExcetion(const Var &begin, const Var &end);
	};

}


#endif //TASK3_TEST_OUT_VAR_LIST_H
