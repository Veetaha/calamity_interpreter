//
// Created by tegeran on 08.07.18.
//
#pragma once

#include "var_substring.h"
#include "var_number.h"

namespace Cala {

    
    String & String::insert(
        const String::size_type & index,
        const cachar_t * const & cString){
        return insert(index, cString, traits_type::length(cString));
    }

    
    String & String::insert(
        const String::size_type & index,
        const cachar_t * const & cString,
        const size_type & amount
    ) {
        reserveMore(amount);
        m_string.insert(index, cString, amount);
        return *this;

    }

    
    String & String::insert(
        const String::size_type & index,
        const String & string) {
        reserveMore(string.size());
        m_string.insert(index, string.m_string);
        return *this;
    }

    
    template<typename T>
    constexpr std::enable_if_t<Vtem::is_numeric_v<T>,
        typename String::size_type>
    String::reservationSizeFor(const T &) {
        return static_cast<size_type>(
            std::numeric_limits<T>::max_digits10  // V~~ for dot and -, 20 for whole part
            ? (std::numeric_limits<T>::max_digits10 + 2 + 20)
            : (std::numeric_limits<T>::digits10 + 2) // for integral values: sign and + 1 naverochku
        );
    }

    
    template<typename TString>
    std::enable_if_t<!Vtem::is_numeric_v<TString>
                     &&  !std::is_pointer_v<TString>,
        typename String::size_type>
    String::reservationSizeFor(const TString & string) {
        return static_cast<size_type>(string.size());
    }


    template< typename String::size_type N>
    constexpr typename String::size_type
    String::reservationSizeFor(const cachar_t (& charArr)[N]) {
        return N;
    }


    String::size_type String::reservationSizeFor(const cachar_t * const & cString) {
        return traits_type::length(cString);
    }


    template<typename T1, typename... TN>
    typename String::size_type
    String::reservationSizeFor(const T1 & item, const TN & ... items) {
        return reservationSizeFor(item) + reservationSizeFor(items...);
    }



    template<typename... TN>
    String & String::push_back(const TN &... items) {
        reserveMore(reservationSizeFor(items...));
        return affiliate(items...);
    }


    template<typename T>
    String & String::affiliate(const T & item) {
        return *this << item;
    }

    
    template<typename T1, typename... TN>
    String & String::affiliate(const T1 & item1, const TN &... items) {
        *this << item1;
        return affiliate(items...);
    }

    
    Point2D<String::size_type>
    String::point2DOf(String::size_type index) const {
        Debug_suppose(index < size());
        Point2D<typename String::size_type> result(1, 0);
        do {
            if (Vtem::equalsOneOf(m_string[index], '\n', '\v')){
                break;
            } else if (m_string[index] == '\t'){
                result.x += 4; // supposed tab size
            } else if (std::isprint(m_string[index])) {
                ++result.x;
            }
            if (--index == npos){
                return result;
            }
        } while(true);
        result.y += std::count_if(
            m_string.begin(),
            m_string.begin() + index + 1,
            [](const auto & character){
                return Vtem::equalsOneOf(character, '\n', '\v');
            }
        );
        return result;
    }


    
    auto String::lineOf(const String::size_type & index) const {
        auto begin(m_string.rfind('\n', index));
        auto end  (m_string.find ('\n', index));
        return Substring(
            (begin == npos ? m_string.begin() : (m_string.begin() + begin + 1)),
            (end   == npos ? m_string.end()   : (m_string.begin() + end))
        );
    }

    auto String::lineOf(const const_iterator & iter) const {
        return lineOf(iter - begin());
    }
    
    template <typename TPredIsSpace>
    String::const_iterator String::findNonSpace(
        const size_type & fromPos,
        const TPredIsSpace & isspace
    ) {
        Debug_suppose(fromPos < size());
        return std::find_if(m_string.begin() + fromPos, m_string.end(), isspace);
    }

    
    template <typename TPredIsSpace>
    String::const_iterator String::rfindNonSpace(
        const size_type & fromPos,
        const TPredIsSpace & isspace
    ) {
        if (isEmpty()) return cend();
        auto traverser(
            fromPos >= length() || fromPos == npos
            ? m_string.cend() - 1
            : m_string.cbegin() + fromPos
        );
        const auto & beginIter(cbegin());
        while (traverser != beginIter){
            if (!isspace(*traverser)){
                return traverser;
            }
            --traverser;
        }
        return isspace(*beginIter) ? cend() : beginIter;
    }

    
    typename String::size_type
    String::find(const cachar_t & character, const size_type & fromPos) const {
        return m_string.find(character, fromPos);
    }

    
    typename String::size_type
    String::rfind(const cachar_t & character, const size_type & position) const {
        return m_string.rfind(character, position);
    }

    
    void String::reserveMore(size_type amount) {
        if (m_string.capacity() < (amount += size())) {
            m_string.reserve(amount);
        }
    }

    
    const cachar_t & String::at(const size_type & index) const & {
        return m_string.at(index);
    }

    
    cachar_t & String::at(const size_type & index) & {
        return m_string.at(index);
    }

    
    cachar_t String::at(const size_type & index) && {
        return m_string.at(index);
    }


    
    const cachar_t & String::operator[](const size_type & index) const & {
        return m_string[index];
    }

    
    cachar_t & String::operator[](const size_type & index) & {
        return m_string[index];
    }

    
    cachar_t String::operator[](const size_type & index) && {
        return m_string[index];
    }

    String & String::operator+=(const Substring & substr){
        reserveMore(substr.size());
        m_string.append(substr.data(), substr.size());
        return *this;
    }

    
    void String::print(ostream & stream) { stream << *this; }



    
    bool String::operator==(const String & other) const {
        return this == &other || m_string == other.m_string;
    }

    
    bool String::operator==(const cachar_t * cString) const {
        auto selfStr(c_str());
        while (*cString && *selfStr) {
            if (!std::char_traits<cachar_t>::eq(*cString ++, *selfStr ++))
                return false;
        }
        return *cString == *selfStr;
    }

    
    bool String::operator!=(const String & other) const {
        return !operator==(other);
    }

    
    bool String::operator!=(const cachar_t * const & cString) const {
        return !operator==(cString);
    }


    
    bool String::operator>=(const String & other) const {
        return this == &other || m_string >= other.m_string;
    }

    
    bool String::operator<=(const String & other) const {
        return this == &other || m_string <= other.m_string;
    }

    
    bool String::operator<(const String & other) const {
        return this != &other && m_string < other.m_string;
    }

    
    bool String::operator>(const String & other) const {
        return this != &other && m_string > other.m_string;
    }

    
    String & String::operator=(const String & other) {
        m_string = other.m_string;
        return *this;
    }

    
    String & String::operator=(String && rvalue) {
        m_string = static_cast<string_base &&>(rvalue.m_string);
        return *this;
    }

    
    String & String::operator=(const string_base & string) {
        m_string = string;
        return *this;
    }

    
    String & String::operator=(string_base && string) {
        m_string = static_cast<string_base &&>(string);
        return *this;
    }

    
    String & String::operator=(const cachar_t * const & cString) {
        m_string = cString;
        return *this;
    }

    
    String & String::operator=(const cachar_t & character) {
        m_string = character;
        return *this;
    }

    
    String & String::operator=(const std::initializer_list<cachar_t> & il) {
        m_string = il;
        return *this;
    }

    
    String & String::operator+=(const String & string) {
        m_string += string.m_string;
        return *this;
    }

    
    String & String::operator+=(const string_base & string) {
        m_string += string;
        return *this;
    }

    
    String & String::operator+=(const cachar_t * const & cString) {
        m_string += cString;
        return *this;
    }

    
    String & String::operator+=(const cachar_t & character) {
        m_string += character;
        return *this;
    }

    
    String & String::operator+=(const std::initializer_list<cachar_t> & il) {
        m_string += il;
        return *this;
    }

    
    Point2D<typename String::size_type>
    String::point2DOf(const String::const_iterator & selfIterator) const {
        return point2DOf(static_cast<size_type>(selfIterator - begin()));
    }

    
    String String::cast(const Number & number) {
        return boost::lexical_cast<string_base>(number);
    }

    
    template<typename T>
    std::enable_if_t<
        !Vtem::is_numeric_v<T>
        ||   std::is_same_v<T, cachar_t>,
    String &> String::operator<<(const T & entity) {
        return *this += entity;
    }

    
    template<typename TNumeric>
    std::enable_if_t<
        Vtem::is_numeric_v<TNumeric>
        && !std::is_same_v<TNumeric, cachar_t>,
    String &> String::operator<<(const TNumeric & entity) {
        m_string += ::boost::lexical_cast<string_base>(entity);
        return *this;
    }


    template <typename TPredIsSpace>
    inline void String::trimBegin(const TPredIsSpace & isspace) {
        m_string.erase(
            m_string.begin(),
            std::find_if_not(m_string.begin(), m_string.end(), isspace)
        );
    }

    template <typename TPredIsSpace>
    inline void String::trimEnd(const TPredIsSpace & isspace) {
        m_string.erase(
            std::find_if_not(m_string.rbegin(), m_string.rend(), isspace).base(),
            m_string.end()
        );
    }

    template <typename TPredIsSpace>
    inline void String::trim(const TPredIsSpace & isspace) {
        trimEnd(isspace);
        trimBegin(isspace);
    }

    template <typename TPredIsSpace>
    inline String String::trimmedBegin(const TPredIsSpace & isspace) const {
        return String(
            std::find_if_not(m_string.begin(), m_string.end(), isspace),
            m_string.end()
        );
    }

    template <typename TPredIsSpace>
    inline String String::trimmedEnd(const TPredIsSpace & isspace) const {
        return String(
            m_string.begin(),
            std::find_if_not(m_string.rbegin(), m_string.rend(), isspace).base()
        );
    }

    template <typename TPredIsSpace>
    inline String String::trimmed(const TPredIsSpace & isspace) const {
        const auto firstNonSpace(std::find_if_not(m_string.begin(), m_string.end(), isspace));
        return firstNonSpace == m_string.end()
            ? String()
            : String(
                firstNonSpace,
                std::find_if_not(m_string.rbegin(), m_string.rend(), isspace).base()
            );
    }

    String & String::append(const String::size_type & amount, const cachar_t & token) & {
        m_string.append(amount, token);
        return *this;
    }

    String & String::append(const cachar_t * const & cString, const String::size_type & size) {
        m_string.append(cString, size);
        return *this;
    }


}


