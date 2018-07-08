//
// Created by tegeran on 01.07.18.
//

#ifndef TASK3_OUT_VAR_SUBSTR_H
#define TASK3_OUT_VAR_SUBSTR_H

#include <algorithm>
#include <boost/range.hpp>
#include <boost/lexical_cast.hpp>
#include "var_string.h"
#include "defs.h"

namespace Calamity {
    template<typename TChar>
    class BasicSubstring {
    public:
        typedef std::basic_string<TChar> SuperString;
        typedef typename SuperString::const_iterator const_iterator;
        typedef typename SuperString::size_type      size_type;
    private:
        const_iterator m_begin;
        const_iterator m_end;
    public:
        // according to Clang tidy, const_iterator is a trivially copyable type
        BasicSubstring(const const_iterator & begin, const const_iterator & end)
            : m_begin(begin), m_end(end) {}

        inline const const_iterator begin() const { return m_begin; }
        inline const const_iterator end()   const { return m_end;   }
        inline size_type size() const
        { return static_cast<size_type>(m_end - m_begin); }

        inline auto amountOf(const TChar & character) const{
            return std::count(m_begin, m_end, character);
        }

        inline bool contains(const TChar & suspect) const {
            return std::find(m_begin, m_end, suspect) != m_end;
        }

        inline double parseDouble() const {
            return boost::lexical_cast<double>(&(*m_begin), size());
        }

        inline bool operator==(const BasicSubstring & other) const
        { return m_begin == other.m_begin && m_end == other.m_end; }

        inline bool operator!=(const BasicSubstring & other) const
        { return !(*this == other); }

        friend inline std::basic_ostream<TChar> & operator<<(
               std::basic_ostream<TChar> & stream,
               const BasicSubstring & self ){
            for (const auto & character : self) stream << character;
            return stream;
        }

        friend inline auto & operator+=(BasicString<TChar> & string,
                                        const BasicSubstring<TChar> & substring){
            string.reserveMore(substring.size());
            string.append(&(*substring.m_begin), substring.size());
            return string;
        }
    };

    template <typename TChar>
    inline BasicSubstring<TChar> makeSubstring(
        const typename BasicString<TChar>::const_iterator & begin,
        const typename BasicString<TChar>::const_iterator & end
    ) { return BasicSubstring<TChar>(begin, end); }

    typedef BasicSubstring<cachar_t> Substring;
}


#endif //TASK3_OUT_VAR_SUBSTR_H
