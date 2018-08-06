//
// Created by tegeran on 27.06.18.
//

#ifndef TASK3_TEST_OUT_VAR_STRING_H
#define TASK3_TEST_OUT_VAR_STRING_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include "point2d.h"
#include "defs.h"
#include "meta.h"


namespace Cala {

    class String {
        string_base m_string;
        friend class Substring;

    public:
        using value_type   = cachar_t;

        using wrapped_type = string_base;
        using char_type    = value_type;

        typedef typename string_base::traits_type                       traits_type;
        typedef typename string_base::size_type                           size_type;
        typedef typename string_base::iterator                             iterator;
        typedef typename string_base::const_iterator                 const_iterator;
        typedef typename string_base::reverse_iterator             reverse_iterator;
        typedef typename string_base::const_reverse_iterator const_reverse_iterator;

        static constexpr const size_type & npos = std::basic_string<cachar_t>::npos;

        meta_inline String && rval() & { return static_cast<String &&>(*this); }

        meta_inline               iterator   begin()       { return m_string.begin();   }
        meta_inline               iterator     end()       { return m_string.end();     }
        meta_inline         const_iterator   begin() const { return m_string.begin();   }
        meta_inline         const_iterator     end() const { return m_string.end();     }
        meta_inline         const_iterator  cbegin() const { return m_string.cbegin();  }
        meta_inline         const_iterator    cend() const { return m_string.cend();    }
        meta_inline       reverse_iterator  rbegin()       { return m_string.rbegin();  }
        meta_inline       reverse_iterator    rend()       { return m_string.rend();    }
        meta_inline const_reverse_iterator  rbegin() const { return m_string.rbegin();  }
        meta_inline const_reverse_iterator    rend() const { return m_string.rend();    }
        meta_inline const_reverse_iterator crbegin() const { return m_string.crbegin(); }
        meta_inline const_reverse_iterator   crend() const { return m_string.crend();   }

        meta_inline String & append(const cachar_t * const & cString, const size_type & size);

        meta_inline String & append(const size_type & amount, const cachar_t & token) &;


        String() = default;

        String(const cachar_t * const & cString) : m_string(cString) {}
        String(const cachar_t & character) : m_string(1, character) {}

        String(String && rvalue)
            : m_string(std::move(rvalue.m_string)) {}

        String(const String & lvalue)
            : m_string(lvalue.m_string) {}

        String(const string_base & lstring)
            : m_string(lstring) {}

        String(string_base && rstring)
            : m_string(std::move(rstring)) {}

        String(const const_iterator & begin, const const_iterator & end)
            : m_string(begin, end) {}



        String(const Var & var);


        inline const string_base & string() const { return m_string; }
        inline       string_base & string()       { return m_string; }

        inline const cachar_t * c_str() const { return m_string.c_str(); }
        inline const cachar_t *  data() const { return m_string.data();  }
        inline       cachar_t *  data()       { return m_string.data();  }

        inline     bool isEmpty() const { return m_string.empty();  }
        inline     bool   empty() const { return m_string.empty();  }
        inline size_type   size() const { return m_string.size();   }
        inline size_type length() const { return m_string.length(); }

        inline void reserve(const size_type & amount) { m_string.reserve(amount); }
        inline size_type capacity() const { return m_string.capacity(); }

        /**
         * @param number    Number to convert to string
         * @return String containing floating point number to it
         * uses boost::lexical_cast<string_base>(number) inside.
         * See its documentation for more details.
         */
        static inline String cast(const Number & number);
        /**
         * @param boolean   Boolean to write to string
         * @return String "true" if boolean == true, otherwise "false"
         */
        static const String & cast(const Boolean & boolean);

        /**
         * @return String "undefined"
         */
        static const String & cast(const Undefined & undefined);
        /**
         * @return String "null"
         */
        static const String & cast(const Null & null);


        /**
         * @param lval  Lvalue list to write to string
         * @return String with list written to it using format
         *  "[item1, item2, item3, ..., itemN]"
         *
         *  Always creates whole new string.
         */
        static String cast(const List & lval);

        /**
         * @param lval  Lvalue list to write to string
         * @return String with list written to it using format
         *  "[item1, item2, item3, ..., itemN]"
         *
         *  If the first item in the list is a string, uses this string and returns it.
         */
        static String cast(List && rval);



        /**
         * @tparam TPredIsSpace == decltype(isspace)
         * @param fromPos   initial position to search from
         * @param isspace   Predicate bool (*)(const cachar_t & c) returns true if c is whitespace
         * @return Iterator to the first non-space character in range [fromPos, size())
         * If the string is empty or contains no whitespaces, returns value == end()
         * If fromPos >= length(), undefined behaviour
         *
         * Whitespaces are defined by std::isspace(character, <global_locale>) by defautl
         */
        template <typename TPredIsSpace = ::Vtem::IsWhitespace<cachar_t> >
        const_iterator  findNonSpace(
            const size_type & fromPos = 0,
            const TPredIsSpace & isspace = TPredIsSpace()
        );

        /**
         * @brief Almost the same as findNonSpace(), but searches backwards.
         * The other difference is in range check:
         * if (fromPos >= length() || fromPos == npos) fromPos = length() - 1;
         */
        template <typename TPredIsSpace = ::Vtem::IsWhitespace<cachar_t> >
        const_iterator  rfindNonSpace(
            const size_type & fromPos = npos,
            const TPredIsSpace & isspace = TPredIsSpace()
        );


        inline size_type  find(
            const cachar_t & character,
            const size_type & fromPos = 0
        ) const;

        inline size_type rfind(
            const cachar_t & character,
            const size_type & position = npos
        ) const;

        inline void reserveMore(size_type amount);

        inline const cachar_t & at(const size_type & index) const &;
        inline       cachar_t & at(const size_type & index) &      ;
        inline       cachar_t   at(const size_type & index) &&     ;

        inline const cachar_t & operator[](const size_type & index) const &;
        inline       cachar_t & operator[](const size_type & index) &      ;
        inline       cachar_t   operator[](const size_type & index) &&     ;


        inline friend ostream & operator<<(ostream & stream, const String & self)
        { return stream << self.m_string; }

        inline void print(ostream & stream);

        static String readFromFile(const char * const & path);

        inline bool operator==(const String & other) const;
        inline bool operator!=(const String & other) const;
        inline bool operator>=(const String & other) const;
        inline bool operator<=(const String & other) const;
        inline bool operator >(const String & other) const;
        inline bool operator <(const String & other) const;


        inline bool operator==(const cachar_t *         cString) const;
        inline bool operator!=(const cachar_t * const & cString) const;


        inline String & operator=(const String &   other);
        inline String & operator=(      String &&  rvalue);
        inline String & operator=(const string_base &   string);
        inline String & operator=(      string_base &&  string);
        inline String & operator=(const cachar_t * const & cString);
        inline String & operator=(const cachar_t & character);
        inline String & operator=(const std::initializer_list<cachar_t> & il);

        inline String & operator+=(const String & string);
        inline String & operator+=(const string_base & string);
        inline String & operator+=(const cachar_t * const & cString);
        inline String & operator+=(const cachar_t & character);
        inline String & operator+=(const std::initializer_list<cachar_t> & il);
        inline String & operator+=(const Substring & substr);


        String & operator+=(const char * cStr);
        String & operator+=(const Var & var);
        friend inline String & operator>>(const Var & var, String & string);

        friend inline string_base operator+=(string_base & basicString, const String & self)
        { return basicString += self.m_string; }

        template<typename... TN>
        String & push_back(const TN & ...items);

        inline String & insert(const size_type & index, const String & string);
        inline String & insert(const size_type & index, const cachar_t * const & cString);
        inline String & insert(
            const size_type & index,
            const cachar_t * const & cString,
            const size_type & amount
        );


        template<typename TNumeric>
        inline std::enable_if_t<
            Vtem::is_numeric_v<TNumeric> && !std::is_same_v<TNumeric, cachar_t>,
        String & > operator<<(const TNumeric & entity);

        template<typename T>
        inline std::enable_if_t<
            !Vtem::is_numeric_v<T> || std::is_same_v<T, cachar_t>,
        String &> operator<<(const T & entity);


        template<typename T>
        inline friend String & operator>>(const T & entity, String & self) {
            Vtem::prependToString(
                self.m_string, entity
            );
            return self;
        }

        inline friend String & operator>>(const String & prep, String & self){
            return self.insert(0, prep);
        }

        /**
         * @brief   Removes all leading space characters from the string.
         * @tparam TPredIsSpace predicate of form bool (*)(cachar_t c) to find out if c is space
         *
         * If string is empty or contains only whitespace characters, string becomes empty.
         */
        template <typename TPredIsSpace = IsWhitespace>
        inline void trimBegin(const TPredIsSpace & isspace = TPredIsSpace());


        /**
         * @brief   Removes all trailing space characters from the string.
         * @tparam TPredIsSpace predicate of form bool (*)(cachar_t c) to find out if c is space
         *
         * If string is empty or contains only whitespace characters, string becomes empty.
         */
        template <typename TPredIsSpace = IsWhitespace>
        inline void trimEnd(const TPredIsSpace & isspace = TPredIsSpace());

        /**
         * @brief   Removes all leading and trailing space characters from the string.
         * @tparam TPredIsSpace predicate of form bool (*)(cachar_t c) to find out if c is space
         *
         * If string is empty or contains only whitespace characters, string becomes empty.
         */
        template <typename TPredIsSpace = IsWhitespace>
        inline void trim(const TPredIsSpace & isspace = TPredIsSpace());


        /**
         * @brief Returns a copy of this string whith removed leading whitespaces.
         * Original string is not affected.
         */
        template <typename TPredIsSpace = IsWhitespace>
        inline String trimmedBegin(const TPredIsSpace & isspace = TPredIsSpace()) const;

        /**
         * @brief Returns a copy of this string whith removed trailing whitespaces.
         * Original string is not affected.
         */
        template <typename TPredIsSpace = IsWhitespace>
        inline String trimmedEnd(const TPredIsSpace & isspace = TPredIsSpace()) const;

        /**
         * @brief Returns a copy of this string whith removed leading and trailing whitespaces.
         * Original string is not affected.
         */
        template <typename TPredIsSpace = IsWhitespace>
        inline String trimmed(const TPredIsSpace & isspace = TPredIsSpace()) const;

        inline Point2D<size_type> point2DOf(size_type index) const;
        inline Point2D<size_type> point2DOf(const const_iterator & selfIterator) const;

        // these methods in fact return BasicSubstring<cachar_t>, but it can only be forward declared
        // thus such a lifehack is used
        inline auto lineOf(const size_type & index) const;
        inline auto lineOf(const const_iterator & iter) const;

    private:

        template<typename T>
        inline constexpr std::enable_if_t<Vtem::is_numeric_v<T>,
        size_type> reservationSizeFor(const T & item);

        template <size_type N>
        inline constexpr size_type reservationSizeFor(const cachar_t (& charArr)[N]);

        template<typename TString>
        inline std::enable_if_t<!Vtem::is_numeric_v<TString>
                         &&  !std::is_pointer_v<TString>,
        size_type> reservationSizeFor(const TString & string);

        inline size_type reservationSizeFor(const cachar_t * const & cString);

        template<typename T1, typename... TN>
        inline size_type reservationSizeFor(const T1 & item, const TN & ... items);


        template<typename T>
        inline String & affiliate(const T & item);

        template<typename T1, typename... TN>
        inline String & affiliate(const T1 & item1, const TN & ... items);

    };



}

#include "var_string_inline_impl.h"



#endif //TASK3_TEST_OUT_VAR_STRING_H