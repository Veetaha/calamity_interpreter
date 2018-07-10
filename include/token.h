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
            PlusAssign,
            MinusAssign,
            MultiplyAssign,
            DivideAssign,
			RemainderAssign,
            PowerAssign,
            BitAndAssign,
            BitOrAssign,
            BitXorAssign,
            BitLShiftAssign,
            Bit3RShiftAssign,
            Bit2RShiftAssign,
            RightCurly,
            LeftCurly,
            BitAnd,
            BitOr,
            BitNot,
            BitXor,
			BitLeftShift,
            Bit3RightShift,
            Bit2RightShift,
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
            SLComment  = lexertl::rules::skip(),
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
            const String::const_iterator & begin,
            const String::const_iterator & end
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


        const cachar_t * typeName() const;
        static const cachar_t * typeName(const Type & type);

        bool operator==(const Token & other) const;

        friend String & operator+=(String & string, const Token & self);
        friend String & operator+=(String & string, const Token & type);

        friend ostream & operator<<(ostream & stream, const Token & self);

        struct TokenType_Name_Regex {
            Token::Type type;
            const cachar_t * const name;
            const cachar_t * const regex;
            TokenType_Name_Regex(
                    const Token::Type & type,
                    const cachar_t * const & name,
                    const cachar_t * const & regex
            ) : type(type), name(name), regex(regex) {}
        };
        static lexertl::basic_rules<cachar_t, cachar_t> makeLexertlRules();


        inline bool hasType(const Type & type) const { return m_type == type; }
    };


}

