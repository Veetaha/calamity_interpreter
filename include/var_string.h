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


namespace Calamity {
    template<typename TChar>
    class BasicSubstring;

    template<typename TChar>
    class BasicString {
        std::basic_string<TChar> m_string;

        friend class BasicSubstring<TChar>;

    public:
        using string_t = std::basic_string<TChar>;

        typedef typename string_t::traits_type traits_type;
        typedef typename string_t::size_type size_type;
        typedef typename string_t::iterator iterator;
        typedef typename string_t::const_iterator const_iterator;
        typedef typename string_t::reverse_iterator reverse_iterator;
        typedef typename string_t::const_reverse_iterator const_reverse_iterator;

        static inline const size_type & npos() {
            return std::basic_string<TChar>::npos;
        }

        inline iterator begin() { return m_string.begin(); }

        inline iterator end() { return m_string.end(); }

        inline const_iterator begin() const { return m_string.begin(); }

        inline const_iterator end() const { return m_string.end(); }

        inline const_iterator cbegin() const { return m_string.cbegin(); }

        inline const_iterator cend() const { return m_string.cend(); }

        inline reverse_iterator rbegin() { return m_string.rbegin(); }

        inline reverse_iterator rend() { return m_string.rend(); }

        inline const_reverse_iterator rbegin() const { return m_string.rbegin(); }

        inline const_reverse_iterator rend() const { return m_string.rend(); }

        inline const_reverse_iterator crbegin() const { return m_string.crbegin(); }

        inline const_reverse_iterator crend() const { return m_string.crend(); }

        inline BasicString & append(const TChar * const & cString, const size_type & size) {
            m_string.append(cString, size);
            return *this;
        }


        BasicString() = default;

        BasicString(const TChar * const & cString) : m_string(cString) {}

        BasicString(BasicString && rvalue)
            : m_string(std::move(rvalue.m_string)) {}

        BasicString(const BasicString & lvalue)
            : m_string(lvalue.m_string) {}

        BasicString(const string_t & lstring)
            : m_string(lstring) {}

        BasicString(string_t && rstring)
            : m_string(std::move(rstring)) {}


        inline const string_t & string() const { return m_string; }

        inline string_t & string() { return m_string; }

        inline TChar * data() { return m_string.data(); }

        inline const TChar * data() const { return m_string.data(); }

        inline bool empty() const { return m_string.empty(); }

        inline size_type size() const { return m_string.size(); }

        inline size_type length() const { return m_string.length(); }

        inline void reserve(const size_type & amount) { m_string.reserve(amount); }

        inline size_type capacity() const { return m_string.capacity(); }

        inline const TChar * c_str() const { return m_string.c_str(); }


        inline size_type rfind(
            const TChar & character,
            const size_type & position = std::basic_string<TChar>::npos
        ) { return m_string.rfind(character, position); }

        inline void reserveMore(const size_type & amount) {
            size_type reservation(size() + amount);
            if (m_string.capacity() < reservation) {
                m_string.reserve(reservation);
            }
        }

        inline const TChar & at(const size_type & index) const & { return m_string.at(index); }

        inline TChar & at(const size_type & index) & { return m_string.at(index); }

        inline TChar at(const size_type & index) const && { return m_string.at(index); }

        inline const TChar & operator[](const size_type & index) const & { return m_string[index]; }

        inline TChar & operator[](const size_type & index) & { return m_string[index]; }

        inline TChar operator[](const size_type & index) const && { return m_string[index]; }


        friend std::basic_ostream<TChar> & operator<<(
            std::basic_ostream<TChar> & stream, const BasicString & self)
        { return stream << self.m_string; }


        void print(std::basic_ostream<TChar> & stream = std::cout) { stream << *this; }

        static BasicString readFromFile(const char * const & path);

        inline bool operator==(const BasicString & other) const { return this == &other || m_string == other.m_string; }

        inline bool operator==(const TChar * cString) const {
            auto selfStr(c_str());
            while (*cString && *selfStr) {
                if (! std::char_traits<TChar>::eq(*cString ++, *selfStr ++))
                    return false;
            }
            return *cString == *selfStr;
        }

        inline bool operator!=(const BasicString & other) const { return ! (*this == other); }

        inline bool operator!=(const TChar * const & cString) const { return ! (*this == cString); }

#define BasicString_defRelationalOperator(OPERATOR)            \
        inline bool operator OPERATOR(const BasicString & other) const \
        { return m_string OPERATOR other.m_string; }                   \


        BasicString_defRelationalOperator(<)

        BasicString_defRelationalOperator(<=)

        BasicString_defRelationalOperator(>=)

        BasicString_defRelationalOperator(>)

        inline BasicString & operator=(const BasicString & other) {
            m_string = other.m_string;
            return *this;
        }

        inline BasicString & operator=(BasicString && rvalue){
            m_string = static_cast<string_t &&>(rvalue.m_string);
            return *this;
        }

        inline BasicString & operator=(const string_t & string) {
            m_string = string;
            return *this;
        }

        inline BasicString & operator=(string_t && string) {
            m_string = std::move(string);
            return *this;
        }

        inline BasicString & operator=(const TChar * const & cString) {
            m_string = cString;
            return *this;
        }

        inline BasicString & operator=(const TChar & character) {
            m_string = character;
            return *this;
        }

        inline BasicString & operator=(const std::initializer_list<TChar> & il) {
            m_string = il;
            return *this;
        }

        inline BasicString & operator+=(const BasicString & string) {
            m_string += string.m_string;
            return *this;
        }

        inline BasicString & operator+=(const string_t & string) {
            m_string += string;
            return *this;
        }

        inline BasicString & operator+=(const TChar * const & cString) {
            m_string += cString;
            return *this;
        }

        inline BasicString & operator+=(const TChar & character) {
            m_string += character;
            return *this;
        }

        inline BasicString & operator+=(const std::initializer_list<TChar> & il) {
            m_string += il;
            return *this;
        }

        friend inline string_t operator+=(string_t & basicString, const BasicString & self){
            return basicString += self.m_string;
        }

        template<typename... TN>
        BasicString & push_back(const TN & ... items);

        inline BasicString & insert(const size_type & index,
                                    const TChar * const & cString
        );

        inline BasicString & insert(const size_type & index,
                                    const TChar * const & cString,
                                    const size_type & amount
        );
        inline BasicString & insert(const size_type & index, const BasicString & string);

        template<typename TNumeric>
        inline std::enable_if_t<
            std_ext::is_numeric_v<TNumeric>
            && ! std::is_same_v<TNumeric, TChar>,
            BasicString &
        >
        operator<<(const TNumeric & entity) {
            std_ext::appendNumberToString(m_string, entity);
            return *this;
        }

        template<typename T>
        inline std::enable_if_t<
            ! std_ext::is_numeric_v<T>
            || std::is_same_v<T, TChar>,
        BasicString &> operator<<(const T & entity) { return *this += entity; }


        template<typename T>
        inline friend BasicString & operator>>(const T & entity, BasicString & self) {
            std_ext::prependToString(
                self.m_string, entity
            );
            return self;
        }

        inline friend BasicString & operator>>(const BasicString & prep, BasicString & self){
            return self.insert(0, prep);
        }


        inline Point2D<size_type> point2DOf(size_type index) const;

        inline Point2D<size_type> point2DOf(const const_iterator & selfIterator) const {
            return point2DOf(
                selfIterator - begin());
        }

        inline auto lineOf(const size_type & index) const;

        inline auto lineOf(const const_iterator & iter) const { return lineOf(iter - begin()); }


    private:

        template<typename T>
        constexpr std::enable_if_t<std_ext::is_numeric_v<T>,
        size_type> reservationSizeFor(const T & item);

        template <size_t N>
        constexpr size_t reservationSizeFor(const TChar (& charArr)[N]);



        template<typename TString>
        std::enable_if_t<! std_ext::is_numeric_v<TString>
                         && ! std::is_pointer_v<TString>
                         && ! std::is_same_v<TString, BasicSubstring<TChar> >,
        size_type> reservationSizeFor(const TString & string);

        size_type reservationSizeFor(const BasicSubstring<TChar> & substring);

        size_type reservationSizeFor(const TChar * const & cString);

        template<typename T1, typename... TN>
        size_type reservationSizeFor(const T1 & item, const TN & ... items);


        template<typename T>
        BasicString & affiliate(const T & item);

        template<typename T1, typename... TN>
        BasicString & affiliate(const T1 & item1, const TN & ... items);

    };

    class Var; // forward declaration

    using String = BasicString<cachar_t>;
    String & operator+=(String & self, const char * cStr);

    String & operator+=(String & self, const Var & var);
    String & operator>>(const Var & var, String & string);

}

#include "var_string_impl.h"



#endif //TASK3_TEST_OUT_VAR_STRING_H