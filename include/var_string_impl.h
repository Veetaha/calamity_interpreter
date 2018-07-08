//
// Created by tegeran on 08.07.18.
//

#ifndef LEXERTL_GEN_VAR_STRING_IMPL_H
#define LEXERTL_GEN_VAR_STRING_IMPL_H

#include "var_substring.h"

namespace Calamity {

    template<typename TChar>
    BasicString<TChar> & BasicString<TChar>::insert(
        const BasicString::size_type & index,
        const TChar * const & cString){
        return insert(index, cString, string_t::traits_type::length(cString));
    }

    template<typename TChar>
    BasicString<TChar> & BasicString<TChar>::insert(
        const BasicString::size_type & index,
        const TChar * const & cString,
        const size_type & amount
    ) {
        reserveMore(amount);
        m_string.insert(index, cString, amount);
        return *this;

    }

    template<typename TChar>
    BasicString<TChar> & BasicString<TChar>::insert(
        const BasicString<TChar>::size_type & index,
        const BasicString<TChar> & string) {
        reserveMore(string.size());
        m_string.insert(index, string.m_string);
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
                     &&  !std::is_same_v<TString, BasicSubstring<TChar> >,
        typename BasicString<TChar>::size_type>
    BasicString<TChar>::reservationSizeFor(const TString & string) {
        return static_cast<size_type>(string.size());
    }

    template<typename TChar>
    template<size_t N>
    constexpr size_t BasicString<TChar>::reservationSizeFor(const TChar (& charArr)[N] ) {
        return N;
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
    auto
    BasicString<TChar>::lineOf(const BasicString<TChar>::size_type & index) const {
        auto begin(m_string.rfind('\n', index));
        auto end  (m_string.find('\n', index));
        return BasicSubstring<TChar>(
            (begin == npos() ? m_string.begin() : (m_string.begin() + begin + 1)),
            (end   == npos() ? m_string.end()   : (m_string.begin() + end))
        );
    }

    template<typename TChar>
    typename BasicString<TChar>::size_type
    BasicString<TChar>::reservationSizeFor(const BasicSubstring<TChar> & substring) {
        return substring.size();
    }

}










#endif //LEXERTL_GEN_VAR_STRING_IMPL_H
