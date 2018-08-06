#pragma once
//
// Created by tegeran on 28/07/18.
//

#include <type_traits>
#include "var_substring.h"
#include "var_string.h"

namespace Cala {


    inline const typename Substring::char_type * Substring::data() const {
        return &(*m_begin);
    }

    
    template <typename TPredIsSpace>
    Substring Substring::trimmed(const String & string, const TPredIsSpace & isspace){
        const auto begin(string.findNonSpace(isspace));
            return begin == string.end()
                ? Substring(string.begin(), string.begin())
                : Substring(begin, string.rfindNonSpace(isspace) + 1);
    }

    
    template <typename TPredIsSpace>
    void Substring::trimBegin(const TPredIsSpace & isspace) {
        m_begin = std::find_if_not(m_begin, m_end, isspace);

    }

    
    template <typename TPredIsSpace>
    void Substring::trimEnd(const TPredIsSpace & isspace) {
        while (m_end != m_begin && isspace(*(m_end - 1))){
            --m_end;
        }
    }

    
    template <typename TPredIsSpace>
    void Substring::trim(const TPredIsSpace & isspace) {
        bool trimmedEnd(trimEnd(isspace));
        return trimBegin(isspace) || trimmedEnd;
    }

    
    typename Substring::size_type Substring::size() const {
        return static_cast<size_type>(m_end - m_begin);
    }

    
    inline bool Substring::isEmpty() const {
        return m_end == m_begin;
    }

    
    const Substring::const_iterator & Substring::begin() const & {
        return m_begin;
    }

    
    const Substring::const_iterator & Substring::end() const & {
        return m_end;
    }

    
    Substring::const_iterator Substring::begin() && {
        return m_begin;
    }

    
    Substring::const_iterator Substring::end() && {
        return m_end;
    }


    void Substring::setBegin(const const_iterator & newBegin){
        m_begin = newBegin;
    }

    
    void Substring::setEnd(const const_iterator & newEnd){
        m_end = newEnd;
    }

    
    auto Substring::amountOf(const char_type & character) const {
        return std::count(m_begin, m_end, character);
    }

    
    bool Substring::operator==(const Substring & other) const {
        return (m_begin == other.m_begin && m_end == other.m_end)
        || (size() == other.size() && std::equal(m_begin, m_end, other.begin()));
    }
    bool Substring::operator!=(const Substring & other) const {
        return !operator==(other);
    }
    bool Substring::operator==(const String & other) const{
        return (m_begin == other.cbegin() && m_end == other.cend())
        || (size() == other.size() && std::equal(m_begin, m_end, other.cbegin()));
    }
    bool Substring::operator!=(const String & other) const {
        return !operator==(other);
    }
    bool operator==(const String & other, const Substring & self){
        return self.operator==(other);
    }
    bool operator!=(const String & other, const Substring & self){
        return self.operator!=(other);
    }
    bool Substring::operator==(cacstr_t other) const{
        auto iter(m_begin);
        while (*other && iter != m_end){
            if (*iter != *other){
                return false;
            }
            ++iter;
            ++other;
        }
        return !*other && iter == m_end;
    }
    bool Substring::operator!=(const cacstr_t & other) const{
        return !operator==(other);
    }
    bool operator==(const cacstr_t & other, const Substring & self){
        return self.operator==(other);
    }
    bool operator!=(const cacstr_t & other, const Substring & self){
        return self.operator!=(other);
    }


    
    bool Substring::contains(const char_type & suspect) const {
        return std::find(m_begin, m_end, suspect) != m_end;
    }



    
    inline auto Substring::toString() const {
        return String(m_begin, m_end);
    }

    /* friend of Substring */
    inline ostream & operator <<(ostream & stream, const Substring & self) {
        for (const auto & character : self) {
            stream << static_cast<ostream::char_type>(character);
        }
        return stream;
    }

    template <typename TPredIsSpace>
    inline Substring Substring::trimmedBegin(const TPredIsSpace & isspace) const {
        Substring copy(*this);
        copy.trimBegin(isspace);
        return copy;
    }


    template <typename TPredIsSpace>
    inline Substring Substring::trimmedEnd(const TPredIsSpace & isspace) const {
        Substring copy(*this);
        copy.trimEnd(isspace);
        return copy;
    }


    template <typename TPredIsSpace>
    inline Substring Substring::trimmed(const TPredIsSpace & isspace) const {
        Substring copy(*this);
        copy.trim(isspace);
        return copy;
    }


}


