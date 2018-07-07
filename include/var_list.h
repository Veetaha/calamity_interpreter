//
// Created by tegeran on 27.06.18.
//

#ifndef TASK3_TEST_OUT_VAR_LIST_H
#define TASK3_TEST_OUT_VAR_LIST_H

#include <vector>
#include <iostream>
#include "message_exception.h"
#include "var_boolean.h"
#include "var_double.h"
#include "var.h"

namespace Calamity {

	class List {
		std::vector<Var> m_vector;
	public:

		typedef std::vector<Var>::iterator iterator;
		typedef std::vector<Var>::const_iterator const_iterator;
		typedef std::vector<Var>::value_type value_type;
		typedef std::vector<Var>::reference reference;
		typedef std::vector<Var>::const_reference const_reference;
		typedef std::vector<Var>::reverse_iterator reverse_iterator;
		typedef std::vector<Var>::const_reverse_iterator const_reverse_iterator;


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

		List() = default;
		List(const List & lvalue);
		List(List && rvalue);

		List & operator=(const List & lvalue);
		List & operator=(List && rvalue);

		List & operator+=(const Var & lvalue);
		List & operator+=(Var && rvalue);

		friend List operator+(const List & leftLval,  const List & rightLval);
		friend List operator+(List &&      leftRval,       List && rightRval);
		friend List operator+(List &&      leftRval,  const List & rightLval);
		friend List operator+(const List & leftLval,       List && rightRval);


		List & push_back(const List & lvalue);
		List & emplace_back(List &&rvalue);

		List & push_front(const List & lvalue);
		List & emplace_front(List && rvalue);

		List & push_back(const Var & lvalue);
		List & emplace_back(Var && rvalue);

		List & push_front(const Var & lvalue);
		List & emplace_front(Var && rvalue);

		explicit operator bool() const;
		bool to_bool() const;
		Boolean toBoolean() const;

		Var & operator[](const Var & index) &;
		Var && operator[](const Var & index) &&;

		void insert(const Var & index, Var && rvalue);
		void insert(const Var & index, const Var & lvalue);
		Var remove(const Var & index);

		List sublist(const Var & begin, const Var & end) &;
		List sublist(const Var & begin, const Var & end) &&;

		friend std::wostream & operator<<(std::wostream & stream, const List & self);

		void print(std::wostream & stream = std::wcout)
		{ stream << *this; }


	private:
		void checkReaderIndex(const Var & index) const; // throws MessageException
		void checkInserterIndex(const Var & index) const; // throws MessageException
		MessageException negativeIndexException(const Double & index) const;
		MessageException indexOutOfBoundsException(const Double & index) const;
		MessageException nonNumericListSubscriptException(const Var &subscript) const;
		MessageException sublistInvalidRangeTypeException(const Var &begin, const Var &end);
		MessageException sublistRidiculousRangeExcetion(const Var &begin, const Var &end);
	};









    template<typename TChar>
    BasicString<TChar> & operator+=(BasicString<TChar> & self, const Var & var) {
        switch (var.type()) {
            case Var::Type::Undefined: {
                return self += "undefined";
            }
            case Var::Type::Boolean: {
                return self += var.boolean().toCString();
            }
            case Var::Type::Number: {
                return self += var.number().toString();
            }
            case Var::Type::String: {
                return self += var.string();
            }
            case Var::Type::List: {        // average var.toString() size == 3
                self.reserve(self.size() + 3 * var.list().size());
                return std_ext::appendContainerToString(
                        self,
                        var.list(),
                        [](BasicString<TChar> & str, const Var & variable) {
                            str += variable;
                        }
                );
            }
            default: {
                Debug_noimpl();
            }
        }
    }

    template <typename TChar>
    BasicString<TChar> & operator>>(const Var & var, BasicString<TChar> & string) {
        switch (var.type()) {
            case Var::Type::Undefined: {
                return "undefined" >> string;
            }
            case Var::Type::Boolean: {
                return var.boolean().toCString() >> string;
            }
            case Var::Type::Number: {
                return var.number().value() >> string;
            }
            case Var::Type::String: {
                return var.string() >> string;
            }
            case Var::Type::List: {               // average var.toString() size == 3
                string.reserve(string.size() + 3 * var.list().size());
                return std_ext::prependToString(
                        string,
                        var.list(),
                        [](String & str, const Var & variable) {
                            str += variable;
                        }
                );
            }
            default: {
                Debug_noimpl();
            }
        }
    }













}


#endif //TASK3_TEST_OUT_VAR_LIST_H
