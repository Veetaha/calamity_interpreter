#include <token.h>
#include <error.h>
#include <cui.h>
#include <lexertl/rules.hpp>

namespace Calamity {

    static const Token::TokenType_Name_Regex grammarInfo[]{
        { Token::Type::Eoi            , u"Eoi"      , u"<no_regex_eoi>"        },
        { Token::Type::Enum           , u"enum"     , uR"(enum)"               },
        { Token::Type::Void           , u"void"     , uR"(void)"               },
        { Token::Type::Try            , u"try"      , uR"(try)"                },
        { Token::Type::Catch          , u"catch"    , uR"(catch)"              },
        { Token::Type::Switch         , u"switch"   , uR"(switch)"             },
        { Token::Type::Case           , u"case"     , uR"(case)"               },
        { Token::Type::This           , u"this"     , uR"(this)"               },
        { Token::Type::Break          , u"break"    , uR"(break)"              },
        { Token::Type::Continue       , u"continue" , uR"(continue)"           },
        { Token::Type::Do             , u"do"       , uR"(do)"                 },
        { Token::Type::In             , u"in"       , uR"(in)"                 },
        { Token::Type::Default        , u"default"  , uR"(default)"            },
        { Token::Type::Delete         , u"delete"   , uR"(delete)"             },
        { Token::Type::Throw          , u"throw"    , uR"(throw)"              },
        { Token::Type::Import         , u"import"   , uR"(import)"             },
        { Token::Type::Finally        , u"finally"  , uR"(finally)"            },
        { Token::Type::Typeof         , u"typeof"   , uR"(typeof)"             },
        { Token::Type::Yield          , u"yield"    , uR"(yield)"              },
        { Token::Type::Var            , u"var"      , uR"(var)"                },
        { Token::Type::Let            , u"let"      , uR"(let)"                },
        { Token::Type::Const          , u"const"    , uR"(const)"              },
        { Token::Type::New            , u"new"      , uR"(new)"                },
        { Token::Type::False          , u"false"    , uR"(false)"              },
        { Token::Type::For            , u"for"      , uR"(for)"                },
        { Token::Type::Function       , u"function" , uR"(function)"           },
        { Token::Type::If             , u"if"       , uR"(if)"                 },
        { Token::Type::Else           , u"else"     , uR"(else)"               },
        { Token::Type::True           , u"true"     , uR"(true)"               },
        { Token::Type::Undefined      , u"undefined", uR"(undefined)"          },
        { Token::Type::Null           , u"null"     , uR"(null)"               },
        { Token::Type::While          , u"while"    , uR"(while)"              },
        { Token::Type::Return         , u"return"   , uR"(return)"             },
        { Token::Type::Endl           , u"endl"     , uR"(endl)"               },
        { Token::Type::Identity       , u"==="      , uR"(===)"                },
        { Token::Type::NotIdentity    , u"!=="      , uR"(!==)"                },
        { Token::Type::Increment      , u"++"       , uR"(\+\+)"               },
        { Token::Type::Decrement      , u"--"       , uR"(\-\-)"               },
        { Token::Type::And            , u"&&"       , uR"(&&)"                 },
        { Token::Type::Assign         , u"="        , uR"(=)"                  },
        { Token::Type::RightCurly     , u"}"        , uR"(\})"                 },
        { Token::Type::LeftCurly      , u"{"        , uR"(\{)"                 },
        { Token::Type::BitAnd         , u"&"        , uR"(&)"                  },
        { Token::Type::BitOr          , u"|"        , uR"(\|)"                 },
        { Token::Type::BitNot         , u"~"        , uR"(~)"                  },
        { Token::Type::BitXor         , u"^"        , uR"(\^)"                 },
        { Token::Type::Coma           , u","        , uR"(,)"                  },
        { Token::Type::Division       , u"/"        , uR"(\/)"                 },
        { Token::Type::Equality       , u"=="       , uR"(==)"                 },
        { Token::Type::GreaterOrEqual , u">="       , uR"(>=)"                 },
        { Token::Type::GreaterThan    , u">"        , uR"(>)"                  },
        { Token::Type::LeftBracket    , u"["        , uR"(\[)"                 },
        { Token::Type::LeftPar        , u"("        , uR"(\()"                 },
        { Token::Type::LessOrEqual    , u"<="       , uR"(<=)"                 },
        { Token::Type::LessThan       , u"<"        , uR"(<)"                  },
        { Token::Type::Minus          , u"-"        , uR"(\-)"                 },
        { Token::Type::Multiplication , u"*"        , uR"(\*)"                 },
        { Token::Type::Neq            , u"!="       , uR"(!=)"                 },
        { Token::Type::Not            , u"!"        , uR"(!)"                  },
        { Token::Type::Or             , u"||"       , uR"(\|\|)"               },
        { Token::Type::Plus           , u"+"        , uR"(\+)"                 },
        { Token::Type::Power          , u"**"       , uR"(\*\*)"               },
        { Token::Type::Ellipsis       , u"..."      , uR"(\.\.\.)"             },
        { Token::Type::Remainder      , u"%"        , uR"(%)"                  },
        { Token::Type::RightBracket   , u"]"        , uR"(\])"                 },
        { Token::Type::RightPar       , u")"        , uR"(\))"                 },
        { Token::Type::Colon          , u":"        , uR"(:)"                  },
        { Token::Type::Semicolon      , u";"        , uR"(;)"                  },
        { Token::Type::Arrow          , u"=>"       , uR"(=>)"                 },
        { Token::Type::Dot            , u"."        , uR"(\.)"                 },
        { Token::Type::Identifier     , u"ID"       , uR"([a-zA-Z_$](\w|\$)*)" },
        { Token::Type::String         , u"STR"      , uR"((\".*?\")|('.*?'))"  },
        { Token::Type::Number         , u"NUMBER"   , uR"(\d+(\.?\d*))"        },
        { Token::Type::Newline        , u"\\n"      , uR"(\n)"                 },
        { Token::Type::MLComment      , u"<ml_comm>", uR"(\/\*.*?\*\/)"        },
        { Token::Type::SLComment      , u"<sl_comm>", uR"(\/\/.*?($|\n))"      },
        { Token::Type::Whitespace     , u"<whitesp>", uR"((\s{-}\n)+)"         }
    };


    bool Token::isKeyword(const Token::Type & suspect) {
        return suspect >= Token::Type::M_FirstKetword && suspect <= Token::Type::M_LastKeyword;
    }


    bool Token::isPunctuation(const Token::Type & suspect) {
        return suspect >= Token::Type::M_FirstPunct && suspect <= Token::Type::M_LastPunct;
    }

    Token::Token(const Token::Type & type, Substring && substr)
        : m_type(type), m_substr(std::move(substr)) {}

    Token::Token(
            const Token::Type & type,
            String::const_iterator && begin,
            String::const_iterator && end)
            : m_type(type), m_substr(std::move(begin), std::move(end)) {}


    bool Token::operator==(const Token & other) const {
        return this == &other || (
                 m_substr == other.m_substr
                && m_type == other.m_type
        );
    }

    const char16_t * Token::u16typeName() const {
        return u16typeName(m_type);
    }

    const char16_t * Token::u16typeName(const Type & type) {
        return grammarInfo[static_cast<int>(type)].name;
    }

    std::wostream & operator<<(std::wostream & stream, const Token & self) {
        typedef Token::Type Type;
        using namespace Cui;

        if (self.isKeyword()) {
            return stream << fgnd(2, 2, 5)
                          << L'{'
                          << bold
                          << self.u16typeName()
                          << bold_off
                          << L'}'
                          << reset;
        } else if (self.isPunctuation()) {
            return stream << fgnd(5, 2, 0)
                          << L'{'
                          << bold
                          << self.u16typeName()
                          << bold_off
                          << L'}'
                          << reset;
        }
        stream << L'{' << bold << self.u16typeName();
        switch (self.type()) {
            case Type::Identifier:
                [[fallthrough]]
            case Type::Number:
            case Type::String:{
                stream << L", " << self.m_substr << bold_off;
                break;
            }
            default: {

            }
        }
        return stream << L'}';
    }

    String & operator+=(String & string, const Token & self) {
        typedef Token::Type Type;
        string += u"{";
        string += self.u16typeName();
        switch (self.type()) {
            case Type::Number:
            case Type::Identifier: {
                (string += u", ") += self.m_substr;
                break;
            }
            case Type::String: {
                ((string += u", \"") += self.m_substr) += u"\"";
                break;
            }
            default: break;
        }
        return string += u'}';
    }

    lexertl::u16rules Token::makeLexertlRules() {
        lexertl::u16rules rules(0); // zero in order to omit default dot_not_newline option
        for (auto * iterator(grammarInfo + 1);
                    iterator != std::end(grammarInfo);
                    ++iterator){
            rules.push(
                iterator->regex,
                static_cast<lexertl::u16rules::id_type>(iterator->type)
            );
        }
        return rules;
    }

}