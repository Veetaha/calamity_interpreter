//
// Created by tegeran on 01.07.18.
//

#ifndef TASK3_OUT_VAR_SUBSTR_H
#define TASK3_OUT_VAR_SUBSTR_H

#include <algorithm>
#include <boost/range.hpp>
#include <boost/lexical_cast.hpp>
#include "var_string.h"

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
        BasicSubstring(const_iterator && begin, const_iterator && end)
            : m_begin(std::move(begin)), m_end(std::move(end)) {}

        inline const const_iterator begin() const { return m_begin; }
        inline const const_iterator end()   const { return m_end;   }
        inline auto size() const          { return m_end - m_begin; }

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



    };

    template <typename TChar>
    inline BasicSubstring<TChar> makeSubstring(
        const typename BasicString<TChar>::const_iterator & begin,
        const typename BasicString<TChar>::const_iterator & end
    ) { return BasicSubstring<TChar>(begin, end); }

    typedef BasicSubstring<char16_t > Substring;

    std::wostream & operator<<(std::wostream & stream, const Substring & self);


}


#endif //TASK3_OUT_VAR_SUBSTR_H
