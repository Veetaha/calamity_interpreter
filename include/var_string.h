//
// Created by tegeran on 27.06.18.
//

#ifndef TASK3_TEST_OUT_VAR_STRING_H
#define TASK3_TEST_OUT_VAR_STRING_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <boost/range.hpp>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "point2d.h"

namespace Calamity {
    template <typename TChar>
    class BasicSubstring;

    template <typename TChar>
	class BasicString {
        std::basic_string<TChar> m_string;

        friend class BasicSubstring<TChar>;
	public:
        using string_t = std::basic_string<TChar>;

        typedef typename string_t::size_type                size_type;
        typedef typename string_t::iterator                 iterator;
        typedef typename string_t::const_iterator           const_iterator;
        typedef typename string_t::reverse_iterator		    reverse_iterator;
        typedef typename string_t::const_reverse_iterator   const_reverse_iterator;

        static inline const size_type & npos()        {
            return std::basic_string<TChar>::npos;
        }
        inline iterator begin()                       { return m_string.begin();   }
        inline iterator end()                         { return m_string.end();     }
        inline const_iterator begin() const           { return m_string.begin();   }
        inline const_iterator end() const             { return m_string.end();     }
        inline const_iterator cbegin() const          { return m_string.cbegin();  }
        inline const_iterator cend() const            { return m_string.cend();    }
        inline reverse_iterator rbegin()              { return m_string.rbegin();  }
        inline reverse_iterator rend()                { return m_string.rend();    }
        inline const_reverse_iterator rbegin() const  { return m_string.rbegin();  }
        inline const_reverse_iterator rend() const    { return m_string.rend();    }
        inline const_reverse_iterator crbegin() const { return m_string.crbegin(); }
        inline const_reverse_iterator crend() const   { return m_string.crend();   }

        BasicString() = default;
        BasicString(const TChar * const & cString) : m_string(cString) {}

        BasicString(BasicString && rvalue)
                : m_string(std::move(rvalue.m_string)){}

        BasicString(const BasicString & lvalue)
                : m_string(lvalue.m_string) {}

        BasicString(const string_t & lstring)
                : m_string(lstring) {}

        BasicString(string_t && rstring)
                : m_string(std::move(rstring)) {}


        inline const string_t & string() const            { return m_string;            }
        inline string_t & string()                        { return m_string;            }

        inline TChar * data()                             { return m_string.data();     }
        inline const TChar * data() const                 { return m_string.data();     }
        inline bool empty()                               { return m_string.empty();    }
        inline size_type size() const                     { return m_string.size();     }
        inline size_type length() const                   { return m_string.length();   }
        inline void reserve(const size_type & amount)     { m_string.reserve(amount);   }
        inline size_type capacity() const                 { return m_string.capacity(); }
        inline const TChar * c_str() const                { return m_string.c_str();    }


        inline size_type rfind(
                const TChar & character,
                const size_type & position = std::basic_string<TChar>::npos
        )
        { return m_string.rfind(character, position); }

        inline void reserveMore(const size_type & amount) {
            size_type reservation(size() + amount);
            if (m_string.capacity() < reservation){
                m_string.reserve(reservation);
            }
        }

        inline const TChar & at(const size_type & index) const & { return m_string.at(index); }
        inline TChar & at(const size_type & index) &             { return m_string.at(index); }
        inline TChar at(const size_type & index) const &&        { return m_string.at(index); }

        inline const TChar & operator[](const size_type & index) const & { return m_string[index]; }
        inline TChar & operator[](const size_type & index) &             { return m_string[index]; }
        inline TChar operator[](const size_type & index) const &&        { return m_string[index]; }


		friend std::basic_ostream<TChar> & operator<<(
		       std::basic_ostream<TChar> & stream, const BasicString & self)
        { return stream << self.m_string; }



        inline friend std::basic_ostream<TChar> & operator<<(
                      std::basic_ostream<TChar> & stream,
                      const boost::iterator_range<
                              typename BasicString<TChar>::const_iterator
                      > & substrRange)
        { for (const auto & character : substrRange) stream << character; return stream; }

		void print(std::basic_ostream<TChar> &stream = std::cout) { stream << *this; }

        static BasicString readFromFile(const char * const & path);

        inline bool operator==(const BasicString & other) const
        { return this == &other || m_string == other.m_string; }

        inline BasicString &operator+=(const boost::iterator_range<BasicString::const_iterator> & strRange);
        inline BasicString &operator+=(const boost::iterator_range<BasicString::iterator> & strRange);

        inline BasicString & operator =(const BasicString & other)               { m_string = other.m_string;    return *this; }
        inline BasicString & operator =(const string_t & string)                 { m_string = string;            return *this; }
        inline BasicString & operator =(string_t && string)                      { m_string = std::move(string); return *this; }
        inline BasicString & operator =(const TChar * const & cString)           { m_string = cString;           return *this; }
        inline BasicString & operator =(const TChar & character)                 { m_string = character;         return *this; }
        inline BasicString & operator =(const std::initializer_list<TChar> & il) { m_string = il;                return *this; }
        inline BasicString & operator+=(const BasicString & string)              { m_string += string.m_string;  return *this; }
        inline BasicString & operator+=(const string_t & string)                 { m_string += string;           return *this; }
        inline BasicString & operator+=(const TChar * const & cString)           { m_string += cString;          return *this; }
        inline BasicString & operator+=(const TChar & character)                 { m_string += character;        return *this; }
        inline BasicString & operator+=(const std::initializer_list<TChar> & il) { m_string += il;               return *this; }

        template <typename... TN>
        BasicString & push_back(const TN &... items);

        inline BasicString &insert(const size_type & index, const TChar * const & cString);

        inline BasicString &insert(const size_type & index, const BasicString & string);

        template <typename TNumeric>
        inline std::enable_if_t<
                std_ext::is_numeric_v<TNumeric>
                && !std::is_same_v<TNumeric, TChar>,
                BasicString &
        >
        operator<<(const TNumeric & entity)
        { std_ext::appendNumberToString(m_string, entity); return *this; }

        template <typename T>
        inline std::enable_if_t<
                !std_ext::is_numeric_v<T>
                ||   std::is_same_v<T, TChar>,
                BasicString &
        >
        operator<<(const T & entity)
        { return *this += entity; }



        template <typename T>
        inline friend BasicString & operator>>(const T & entity, BasicString & self)
        { std_ext::prependToString(self.m_string, entity); return self; }


        inline Point2D<size_type> point2DOf(size_type index) const;
        inline Point2D<size_type> point2DOf(const const_iterator & selfIterator) const
        { return point2DOf(selfIterator - begin());    }

        inline boost::iterator_range<const_iterator>
        lineOf(const size_type & index) const;
        inline auto lineOf(const const_iterator & iter) const { return lineOf(iter - begin()); }


    private:

        template<typename T>
        constexpr std::enable_if_t<std_ext::is_numeric_v<T>, size_type>
        reservationSizeFor(const T & item);

        template<typename TString>
        std::enable_if_t<!std_ext::is_numeric_v<TString>
                         &&  !std::is_pointer_v<TString>
                         &&  !std::is_same_v<TString, boost::iterator_range<const_iterator> >,
                         size_type>
        reservationSizeFor(const TString & string);

        size_type reservationSizeFor(const boost::iterator_range<const_iterator> & strRange);
        size_type reservationSizeFor(const TChar * const & cString);

        template <typename T1, typename... TN>
        size_type reservationSizeFor(const T1 & item, const TN & ... items);


        template <typename T>
        BasicString & affiliate(const T & item);

        template <typename T1, typename... TN>
        BasicString & affiliate(const T1 & item1, const TN &... items);

	};



    template<typename TChar>
    BasicString<TChar> & BasicString<TChar>::insert(const BasicString::size_type & index, const TChar * const & cString) {
        size_type strlen(std::char_traits<TChar>::length(cString));
        m_string.reserve(strlen);
        m_string.insert(index, cString, strlen);
        return *this;

    }

    template<typename TChar>
    BasicString<TChar> & BasicString<TChar>::insert(
            const BasicString<TChar>::size_type & index,
            const BasicString<TChar> & string) {
        m_string.reserve(string.size());
        m_string.insert(index, string);
        return *this;
    }

    template<typename TChar>
    BasicString<TChar> & BasicString<TChar>::operator+=(
            const boost::iterator_range<BasicString<TChar>::iterator> & strRange
    ) {
        Debug_suppose(strRange.begin() <= strRange.end());
        reserveMore(static_cast<BasicString<TChar>::size_type>(strRange.end() - strRange.begin()));
        m_string.append(strRange.begin(), strRange.end());
        return *this;
    }

    template<typename TChar>
    BasicString<TChar> & BasicString<TChar>::operator+=(
            const boost::iterator_range<BasicString<TChar>::const_iterator> & strRange
    ) {
        Debug_suppose(strRange.begin() <= strRange.end());
        reserveMore(static_cast<BasicString<TChar>::size_type>(strRange.end() - strRange.begin()));
        m_string.append(strRange.begin(), strRange.end());
        return *this;
    }


    template<typename TChar>
    template<typename T>
    constexpr std::enable_if_t<std_ext::is_numeric_v<T>,
            typename BasicString<TChar>::size_type>
    BasicString<TChar>::reservationSizeFor(const T & item) {
        return static_cast<size_type>(
            std::numeric_limits<T>::max_digits10  // V~~ for dot and -, 20 for whole part
            ? (std::numeric_limits<T>::max_digits10 + 2 + 20)
            : (std::numeric_limits<T>::digits10 + 2) // for integral values: sign and + 1 naverochku
        );
    }

    template <typename TChar>
    template<typename TString>
    std::enable_if_t<!std_ext::is_numeric_v<TString>
                     &&  !std::is_pointer_v<TString>
                     &&  !std::is_same_v<
                           TString,
                           boost::iterator_range<typename BasicString<TChar>::const_iterator>
                     >,
                     typename BasicString<TChar>::size_type>
    BasicString<TChar>::reservationSizeFor(const TString & string) {
        return static_cast<size_type>(string.size());
    }

    template<typename TChar>
    typename BasicString<TChar>::size_type BasicString<TChar>::reservationSizeFor
            (const TChar * const & cString) {
        return std::char_traits<TChar>::length(cString);
    }

    template<typename TChar>
    template<typename T1, typename... TN>
    typename BasicString<TChar>::size_type
    BasicString<TChar>::reservationSizeFor(const T1 & item, const TN & ... items) {
        return reservationSizeFor(item) + reservationSizeFor(items...);
    }


    template<typename TChar>
    template<typename... TN>
    BasicString<TChar> & BasicString<TChar>::push_back(const TN & ... items) {
        reserveMore(reservationSizeFor(items...));
        return affiliate(items...);
    }

    template<typename TChar>
    template<typename T>
    BasicString<TChar> & BasicString<TChar>::affiliate(const T & item) {
        return *this << item;
    }

    template<typename TChar>
    template<typename T1, typename... TN>
    BasicString<TChar> & BasicString<TChar>::affiliate(const T1 & item1, const TN & ... items) {
        static_cast<void>(*this << item1);
        return affiliate(items...);
    }

    template<typename TChar>
    Point2D<typename BasicString<TChar>::size_type>
    BasicString<TChar>::point2DOf(BasicString<TChar>::size_type index) const {
        Debug_suppose(index < size());
        Point2D<typename BasicString<TChar>::size_type> result(1, 0);
        while(index != npos()){
            if (std_ext::equalsOneOf(m_string[index], '\n', '\v')){
                break;
            } else if (m_string[index] == '\t'){
                result.x += 4;
            } else if (std::isprint(m_string[index])) {
                ++result.x;
            }
            --index;
        }
        while (index != npos()) {
            if (std_ext::equalsOneOf(m_string[index], '\n', '\v')){
                ++result.y;
            }
            --index;
        }
        return result;
    }

    template<typename TChar>
    boost::iterator_range<typename BasicString<TChar>::const_iterator>
    BasicString<TChar>::lineOf(const BasicString<TChar>::size_type & index) const {
        auto begin(m_string.rfind('\n', index));
        auto end  (m_string.find('\n', index));
        return boost::make_iterator_range(
            (begin == npos() ? m_string.begin() : (m_string.begin() + begin + 1)),
            (end   == npos() ? m_string.end()   : (m_string.begin() + end))
        );
    }

    template<typename TChar>
    typename BasicString<TChar>::size_type
    BasicString<TChar>::reservationSizeFor(
            const boost::iterator_range<typename BasicString<TChar>::const_iterator> & strRange) {
        return strRange.size();
    }

    typedef BasicString<char16_t> String;
    inline const char16_t endl = '\n';

    std::wostream & operator<<(std::wostream & stream, const String & self);
    std::wostream & operator<<(std::wostream & stream, const char16_t * u16CString);

}



#endif //TASK3_TEST_OUT_VAR_STRING_H