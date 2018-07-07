#pragma once

#include <boost/range.hpp>
#include <lexertl/match_results.hpp>
#include <lexertl/rules.hpp>
#include "point2d.h"
#include "var.h"
#include "var_string.h"
#include "var_substring.h"

namespace Calamity {
	class Token {
	public:
        enum class Type {
            Eoi = lexertl::rules::skip(),
            Enum = 1,
            // 0 is reserved for eoi
            Void,
            Try,
            Catch,
            Switch,
            Case,
            This,
            Break,
            Continue,
            Do,
            In,
            Default,
            Delete,
            Throw,
            Import,
            Finally,
            Typeof,
            Yield,
            Var,
            Let,
            Const,
            New,
            False,
            For,
            Function,
            If,
            Else,
            True,
            Undefined,
            Null,
            While,
            Return,
            Endl,
            Identity,
            NotIdentity,
            Increment,
            Decrement,
            And,
            Assign,
            RightCurly,
            LeftCurly,
            BitAnd,
            BitOr,
            BitNot,
            BitXor,
            Coma,
            Division,
            Equality,
            GreaterOrEqual,
            GreaterThan,
            LeftBracket,
            LeftPar,
            LessOrEqual,
            LessThan,
            Minus,
            Multiplication,
            Neq,
            Not,
            Or,
            Plus,
            Power,
            Ellipsis,
            Remainder,
            RightBracket,
            RightPar,
            Colon,
            Semicolon,
            Arrow,
            Dot,
            Identifier,
            String,
            Number,
            Newline,
            MLComment,
            SLComment  = lexertl::rules::npos(),
            Whitespace = lexertl::rules::skip(),
            M_FirstKetword = static_cast<int>(Type::Enum),
            M_LastKeyword  = static_cast<int>(Type::Endl),
            M_FirstPunct   = static_cast<int>(Type::Identity),
            M_LastPunct    = static_cast<int>(Type::Dot)
        };
    private:
		Type m_type;
		Substring m_substr;
	public:
	    Token(
            const Type & type,
            String::const_iterator && begin,
            String::const_iterator && end
        );
	    Token(const Type & type, Substring && substr);


        inline const Substring & substr() const { return m_substr;         }
        inline auto begin() const               { return m_substr.begin(); }
        inline auto end()  const                { return m_substr.end();   }


        static bool isKeyword(const Type & suspect);
        static bool isPunctuation(const Type & suspect);
        inline bool isPunctuation() const      { return isPunctuation(m_type); }
        inline bool isKeyword() const          { return isKeyword(m_type);     }
        inline Type type() const               { return m_type;                }
        inline void setType(const Type & type) { m_type = type;                }


        const char16_t * u16typeName() const;
        static const char16_t * u16typeName(const Type & type);

        bool operator==(const Token & other) const;

        friend String & operator+=(String & string, const Token & self);
        friend String & operator+=(String & string, const Token & type);

        friend std::wostream & operator<<(std::wostream & stream, const Token & self);

        struct TokenType_Name_Regex {
            Token::Type type;
            const char16_t * const name;
            const char16_t * const regex;
            TokenType_Name_Regex(
                    const Token::Type & type,
                    const char16_t * const & name,
                    const char16_t * const & regex
            ) : type(type), name(name), regex(regex) {}
        };

        static lexertl::u16rules makeLexertlRules();


        inline bool isSemicolon() const { return m_type == Type::Semicolon; }

    };





#define TOKEN_VOLATILE_TYPES 4
#define TOKEN_TOTAL_TOKEN_TYPES (sizeof(LEXEME_NAMES) / sizeof(LEXEME_NAMES[0]))
#define TOKEN_TOTAL_KEYWORDS (((TOKEN_TOTAL_TOKEN_TYPES) - TOKEN_VOLATILE_TYPES))
// Global spreadsheet of kewords


}