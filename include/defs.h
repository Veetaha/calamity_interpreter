#pragma once
#define CALA_CHAR_TYPE  wchar_t
#define CALA_FLOAT_TYPE double
#define CALA_INT_TYPE   int64_t
#define CALA_OSTREAM    ::std::wcout
#define ca(STRING_OR_CHAR_LITERAL) \
Macro_concatenate(Macro_charPrefix(wchar_t), STRING_OR_CHAR_LITERAL)

// #define NO_DEBUG
// #define NO_DEBUG_OUTPUT
#define DEBUG_CHAR_TYPE CALA_CHAR_TYPE
#define DEBUG_OSTREAM CALA_OSTREAM
// #define DEBUG_TRACE_CALL_TOKEN   ca('*')
// #define DEBUG_TRACE_RETURN_TOKEN ca('-')

#include "error.h"

#include <iostream>
#include <locale>
#include <fstream>
#include <vector>
#include "meta.h"
#include "var_null.h"
#include "var_undefined.h"
#include "vtem.h"
#include "point2d.h"
#include "not_null_ptr.h"
#include "tree.h"

namespace Cala {
    using cachar_t  = CALA_CHAR_TYPE;
    using float_t   = CALA_FLOAT_TYPE;
    using int_t     = CALA_INT_TYPE;
    using cacstr_t  = const cachar_t *;

    template <typename T>
    using Tree         = Vtem::Tree<T>;
    template <typename TDistance>
    using Point2D      = Vtem::Point2D<TDistance>;
    using IsWhitespace = Vtem::IsWhitespace<cachar_t>;

    class LexicalInfo;
    class GrammaticalInfo;
    class AstNode;
    class Parser;

    // Main types:
    class Var;      // Dynamically typed container
    // Possible types:
    class Number;
    class Boolean;
    class String;
    class List;
    class Function;
    class Object;

    // Supplementary types:
    class Substring;
    template <typename TInt>
    class BasicInteger;

    using Integer               = BasicInteger<int_t>;
    using string_base           = std::basic_string<cachar_t>;
    using list_base             = std::vector<Var>;
    using string_const_iterator = string_base::const_iterator;
    using string_iterator       = string_base::iterator;
    using list_const_iterator   = list_base::const_iterator;
    using list_iterator         = list_base::iterator;

    using ios           = std::basic_ios          <cachar_t>;
    using streambuf     = std::basic_streambuf    <cachar_t>;
    using istream       = std::basic_istream      <cachar_t>;
    using ostream       = std::basic_ostream      <cachar_t>;
    using iostream      = std::basic_iostream     <cachar_t>;
    using stringbuf     = std::basic_stringbuf    <cachar_t>;
    using istringstream = std::basic_istringstream<cachar_t>;
    using ostringstream = std::basic_ostringstream<cachar_t>;
    using stringstream  = std::basic_stringstream <cachar_t>;
    using filebuf       = std::basic_filebuf      <cachar_t>;
    using ifstream      = std::basic_ifstream     <cachar_t>;
    using ofstream      = std::basic_ofstream     <cachar_t>;
    using fstream       = std::basic_fstream      <cachar_t>;

    constexpr const cachar_t endl = ca('\n');
    inline ostream & conout = CALA_OSTREAM;

    // replace "C" locale with local locale
    inline const auto _B_E_F_O_R_E___M_A_I_N_ = [](){
        std::locale::global(std::locale(""));
        return false;
    }();

    inline ostream & operator<<(ostream & stream, const char * cString){
        while (*cString) stream << static_cast<cachar_t>(*cString++);
        return stream;
    }

    template <typename TTarget, typename TSource>
    meta_inline decltype(auto) cast(const TSource & source){
        static_assert(!std::is_reference_v<TTarget>);
        return TTarget::cast(source);
    }

    template <typename TTarget>
    TTarget cast(const TTarget & source) = delete;

}




