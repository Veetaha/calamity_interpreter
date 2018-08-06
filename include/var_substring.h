#pragma once
//
// Created by tegeran on 01.07.18.
//

#include <algorithm>
#include <boost/range.hpp>
#include <boost/lexical_cast.hpp>
#include "defs.h"

namespace Cala {
    class Substring {
    public:
        typedef string_const_iterator const_iterator;

        typedef size_t     size_type;
        typedef cachar_t  value_type;
        typedef value_type char_type;

    private:
        const_iterator m_begin;
        const_iterator m_end;
    public:
        Substring() = default;

        // according to Clang tidy, const_iterator is a trivially copyable type
        Substring(const const_iterator & begin, const const_iterator & end)
            : m_begin(begin), m_end(end) {}

        inline const const_iterator & begin() const &;
        inline const const_iterator & end()   const &;
        inline const_iterator begin() &&;
        inline const_iterator end() &&;

        inline void setBegin(const const_iterator & newBegin);
        inline void setEnd(const const_iterator & newEnd);

        /**
         * @return String containing copy of this substring
         */
        inline auto toString() const;

        /**
         * @return Pointer to the memory begin iterator points to.
         */
        inline const char_type * data() const;

        /**
         * @return Amount of characters in range of [begin, end) of this substring
         */
        inline size_type size() const;
        /**
         * @return Efectively equivalent to: [size() == 0] or [begin() == end()]
         */
        inline bool isEmpty() const;

        /**
         * @brief Returns the amount of characters located in this substring
         * @param character    Character to locate
         * @return Amount of characters in the substring, 0 if no such character
         * was found in the string.
         *
         *  auto stands for the return type of std::count(begin(), end(), character)
         */
        inline auto amountOf(const char_type & character) const;

        /**
         * @param suspect   Character to test on
         * @return True if this substring contains the suspect, false otherwise
         */
        inline bool contains(const char_type & suspect) const;

        /**
         * @brief Returns new substring based on given string without leading and trailing spaces.
         *
         * @param isspace   Predicate of form bool (*)(const char_type &c) returns true if c is whitespace
         * @param string    String to get new substring from
         * @return  Substring with begin and end iterators pointing to the
         * first non-space characters found from the beginning of the string
         * and from the ending of the string respectively.
         *
         * If string is empty or contains only whitespace characters, then
         * an empty substring pointing to the very first character is returned.
         * Whitespaces are defined by std::isspace(character, <global_locale>) by default
         */
        template <typename TPredIsSpace = IsWhitespace>
        static inline Substring trimmed(
            const String & string,
            const TPredIsSpace & isspace = TPredIsSpace()
        );

        /**
         * @brief Moves begin and end iterators to discard leading and trailing whitespaces.
         * @isspace Predicate in form of bool (*)(const char_type &) to recognize whitespaces
         *
         * Assigns end iterator to begin if substring contains only whitespace characters.
         * Does nothing if substring is empty or contains no leading and trailing whitespaces.
         * Whitespaces are defined by std::isspace(character, <global_locale>) by default.
         */
        template <typename TPredIsSpace = IsWhitespace>
        void trim(const TPredIsSpace & isspace = TPredIsSpace());

        /**
         * @brief Moves begin iterator to discard leading whitespaces.
         * @isspace Predicate in form of bool (*)(const char_type &) to recognize whitespaces
         *
         * Assigns begin iterator to end if substring contains only whitespace characters.
         * Does nothing if substring is empty or contains no leading whitespaces.
         * Whitespaces are defined by std::isspace(character, <global_locale>) by default
         */
        template <typename TPredIsSpace = IsWhitespace>
        void trimBegin(const TPredIsSpace & isspace = TPredIsSpace());

        /**
         * @brief Moves end iterator to discard trailing whitespaces.
         * @isspace Predicate in form of bool (*)(const char_type &) to recognize whitespaces
         *
         * Assigns end iterator to begin if substring contains only whitespace characters.
         * Does nothing if substring is empty or contains no trailing whitespaces.
         * Whitespaces are defined by std::isspace(character, <global_locale>) by default.
         */
        template <typename TPredIsSpace = IsWhitespace>
        void trimEnd(const TPredIsSpace & isspace = TPredIsSpace());

        /**
         * @brief  Same as trimBegin(), but doesn't change underlying iterators
         * and returns String with the propper trimmed begin iterator.
         */
        template <typename TPredIsSpace = IsWhitespace>
        inline Substring trimmedBegin(const TPredIsSpace & isspace = TPredIsSpace()) const;

        /**
         * @brief  Same as trimBegin(), but doesn't change underlying iterators
         * and returns String with the propper trimmed end iterator.
         */
        template <typename TPredIsSpace = IsWhitespace>
        inline Substring trimmedEnd(const TPredIsSpace & isspace = TPredIsSpace()) const;

        /**
         * @brief  Same as trimBegin(), but doesn't change underlying iterators
         * and returns String with the propper trimmed begin and end iterator.
         */
        template <typename TPredIsSpace = IsWhitespace>
        inline Substring trimmed(const TPredIsSpace & isspace = TPredIsSpace()) const;

        inline bool operator==(const Substring & other) const;
        inline bool operator!=(const Substring & other) const;
        inline bool operator==(const String & other) const;
        inline bool operator!=(const String & other) const;
        inline friend bool operator==(const String & other, const Substring & self);
        inline friend bool operator!=(const String & other, const Substring & self);
        inline bool operator==(cacstr_t other) const;
        inline bool operator!=(const cacstr_t & other) const;
        inline friend bool operator==(const cacstr_t & other, const Substring & self);
        inline friend bool operator!=(const cacstr_t & other, const Substring & self);


        friend inline
        ostream & operator<<(ostream & stream, const Substring & self );
    };
}
#include "var_substring_inline_impl.h"

