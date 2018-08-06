#include <token.h>
#include <error.h>
#include <cui.h>
#include <lexertl/rules.hpp>

namespace Cala {

    static const Token::TokenType_Name_Regex grammarInfo[]{
        { Token::Type::Eoi             , ca("EOI"       ), ca("<no_regex_eoi>"        )},
        { Token::Type::Enum            , ca("enum"      ), ca(R"(enum)"               )},
        { Token::Type::Void            , ca("void"      ), ca(R"(void)"               )},
        { Token::Type::Try             , ca("try"       ), ca(R"(try)"                )},
        { Token::Type::Catch           , ca("catch"     ), ca(R"(catch)"              )},
        { Token::Type::Switch          , ca("switch"    ), ca(R"(switch)"             )},
        { Token::Type::Case            , ca("case"      ), ca(R"(case)"               )},
        { Token::Type::This            , ca("this"      ), ca(R"(this)"               )},
        { Token::Type::Break           , ca("break"     ), ca(R"(break)"              )},
        { Token::Type::Continue        , ca("continue"  ), ca(R"(continue)"           )},
        { Token::Type::Do              , ca("do"        ), ca(R"(do)"                 )},
        { Token::Type::In              , ca("in"        ), ca(R"(in)"                 )},
        { Token::Type::Default         , ca("default"   ), ca(R"(default)"            )},
        { Token::Type::Delete          , ca("delete"    ), ca(R"(delete)"             )},
        { Token::Type::Throw           , ca("throw"     ), ca(R"(throw)"              )},
        { Token::Type::Import          , ca("import"    ), ca(R"(import)"             )},
        { Token::Type::Export          , ca("export"    ), ca(R"(export)"             )},
        { Token::Type::Typeof          , ca("typeof"    ), ca(R"(typeof)"             )},
        { Token::Type::Yield           , ca("yield"     ), ca(R"(yield)"              )},
        { Token::Type::Var             , ca("var"       ), ca(R"(var)"                )},
        { Token::Type::Let             , ca("let"       ), ca(R"(let)"                )},
        { Token::Type::Const           , ca("const"     ), ca(R"(const)"              )},
        { Token::Type::New             , ca("new"       ), ca(R"(new)"                )},
        { Token::Type::False           , ca("false"     ), ca(R"(false)"              )},
        { Token::Type::For             , ca("for"       ), ca(R"(for)"                )},
        { Token::Type::Function        , ca("function"  ), ca(R"(function)"           )},
        { Token::Type::If              , ca("if"        ), ca(R"(if)"                 )},
        { Token::Type::Else            , ca("else"      ), ca(R"(else)"               )},
        { Token::Type::True            , ca("true"      ), ca(R"(true)"               )},
        { Token::Type::Undefined       , ca("undefined" ), ca(R"(undefined)"          )},
        { Token::Type::Null            , ca("null"      ), ca(R"(null)"               )},
        { Token::Type::While           , ca("while"     ), ca(R"(while)"              )},
        { Token::Type::Return          , ca("return"    ), ca(R"(return)"             )},
        { Token::Type::Async           , ca("async"     ), ca(R"(async)"              )},
        { Token::Type::Await           , ca("await"     ), ca(R"(await)"              )},
        { Token::Type::Of              , ca("of"        ), ca(R"(of)"                 )},
        { Token::Type::Endl            , ca("endl"      ), ca(R"(endl)"               )},
        { Token::Type::Infinity        , ca("Infinity"  ), ca(R"(Infinity)"           )},
        { Token::Type::NaN             , ca("NaN"       ), ca(R"(NaN)"                )},
        { Token::Type::Instanceof      , ca("instanceof"), ca(R"(instanceof)"         )},
        { Token::Type::Identity        , ca("==="       ), ca(R"(===)"                )},
        { Token::Type::NotIdentity     , ca("!=="       ), ca(R"(!==)"                )},
        { Token::Type::Increment       , ca("++"        ), ca(R"(\+\+)"               )},
        { Token::Type::Decrement       , ca("--"        ), ca(R"(\-\-)"               )},
        { Token::Type::LogAnd          , ca("&&"        ), ca(R"(&&)"                 )},
        { Token::Type::Assign          , ca("="         ), ca(R"(=)"                  )},
        { Token::Type::PlusAssign      , ca("+="        ), ca(R"(\+=)"                )},
        { Token::Type::MinusAssign     , ca("-="        ), ca(R"(\-=)"                )},
        { Token::Type::MultiplyAssign  , ca("*="        ), ca(R"(\*=)"                )},
        { Token::Type::DivideAssign    , ca("/="        ), ca(R"(\/=)"                )},
        { Token::Type::RemainderAssign , ca("%="        ), ca(R"(%=)"                 )},
        { Token::Type::PowerAssign     , ca("**="       ), ca(R"(\*\*=)"              )},
        { Token::Type::BitAndAssign    , ca("&="        ), ca(R"(&=)"                 )},
        { Token::Type::BitOrAssign     , ca("|="        ), ca(R"(\|=)"                )},
        { Token::Type::BitXorAssign    , ca("^="        ), ca(R"(\^=)"                )},
        { Token::Type::BitLShiftAssign , ca("<<="       ), ca(R"(<<=)"                )},
        { Token::Type::Bit3RShiftAssign, ca(">>>="      ), ca(R"(>>>=)"               )},
        { Token::Type::Bit2RShiftAssign, ca(">>="       ), ca(R"(>>=)"                )},
        { Token::Type::RightCurly      , ca("}"         ), ca(R"(\})"                 )},
        { Token::Type::LeftCurly       , ca("{"         ), ca(R"(\{)"                 )},
        { Token::Type::BitAnd          , ca("&"         ), ca(R"(&)"                  )},
        { Token::Type::BitOr           , ca("|"         ), ca(R"(\|)"                 )},
        { Token::Type::BitNot          , ca("~"         ), ca(R"(~)"                  )},
        { Token::Type::BitXor          , ca("^"         ), ca(R"(\^)"                 )},
        { Token::Type::BitLeftShift    , ca("<<"        ), ca(R"(<<)"                 )},
        { Token::Type::Bit3RightShift  , ca(">>"        ), ca(R"(>>)"                 )},
        { Token::Type::Bit2RightShift  , ca(">>>"       ), ca(R"(>>>)"                )},
        { Token::Type::Coma            , ca(","         ), ca(R"(,)"                  )},
        { Token::Type::Division        , ca("/"         ), ca(R"(\/)"                 )},
        { Token::Type::Equality        , ca("=="        ), ca(R"(==)"                 )},
        { Token::Type::GreaterOrEqual  , ca(">="        ), ca(R"(>=)"                 )},
        { Token::Type::GreaterThan     , ca(">"         ), ca(R"(>)"                  )},
        { Token::Type::LeftBracket     , ca("["         ), ca(R"(\[)"                 )},
        { Token::Type::LeftPar         , ca("("         ), ca(R"(\()"                 )},
        { Token::Type::LessOrEqual     , ca("<="        ), ca(R"(<=)"                 )},
        { Token::Type::LessThan        , ca("<"         ), ca(R"(<)"                  )},
        { Token::Type::Minus           , ca("-"         ), ca(R"(\-)"                 )},
        { Token::Type::Asterisk        , ca("*"         ), ca(R"(\*)"                 )},
        { Token::Type::NotEquality     , ca("!="        ), ca(R"(!=)"                 )},
        { Token::Type::LogNot          , ca("!"         ), ca(R"(!)"                  )},
        { Token::Type::LogOr           , ca("||"        ), ca(R"(\|\|)"               )},
        { Token::Type::Plus            , ca("+"         ), ca(R"(\+)"                 )},
        { Token::Type::Power           , ca("**"        ), ca(R"(\*\*)"               )},
        { Token::Type::Ellipsis        , ca("..."       ), ca(R"(\.\.\.)"             )},
        { Token::Type::Remainder       , ca("%"         ), ca(R"(%)"                  )},
        { Token::Type::RightBracket    , ca("]"         ), ca(R"(\])"                 )},
        { Token::Type::RightPar        , ca(")"         ), ca(R"(\))"                 )},
        { Token::Type::Colon           , ca(":"         ), ca(R"(:)"                  )},
        { Token::Type::Semicolon       , ca(";"         ), ca(R"(;)"                  )},
        { Token::Type::Arrow           , ca("=>"        ), ca(R"(=>)"                 )},
        { Token::Type::Dot             , ca("."         ), ca(R"(\.)"                 )},
        { Token::Type::QuestionMark    , ca("?"         ), ca(R"(\?)"                 )},
        { Token::Type::Identifier      , ca("ID"        ), ca(R"((\w{-}\d|$)(\w|\$)*)")},
        { Token::Type::String          , ca("STRING"    ), ca(R"(\"(\\.|[^"\\])*\"|\'(\\.|[^'\\])*\')")},
        { Token::Type::Number          , ca("NUMBER"    ), ca(R"(\d+\.?\d*|\.\d+)")},
        { Token::Type::Newline         , ca("\\n"       ), ca(R"(\n)"                 )},
        { Token::Type::MLComment       , ca("<ml_comm>" ), ca(R"(\/\*.{+}\n*?\*\/)"   )},
        { Token::Type::SLComment       , ca("<sl_comm>" ), ca(R"(\/\/[^\n]*?$)"       )},
        { Token::Type::Whitespace      , ca("<whitesp>" ), ca(R"((\s{-}\n)+)"         )}
    };


    bool Token::isKeyword(const Token::Type & suspect) {
        return suspect >= Token::Type::M_FirstKetword && suspect <= Token::Type::M_LastKeyword;
    }


    bool Token::isPunctuation(const Token::Type & suspect) {
        return suspect >= Token::Type::M_FirstPunct && suspect <= Token::Type::M_LastPunct;
    }

    Token::Token(const Token::Type & type, Substring && substr)
        : m_type(type), m_substr(substr) {}

    Token::Token(
            const Token::Type & type,
            const String::const_iterator & begin,
            const String::const_iterator & end)
            : m_type(type), m_substr(begin, end) {}


    bool Token::operator==(const Token & other) const {
        return this == &other
        || (m_type == other.m_type && m_substr == other.m_substr);
    }

    const cachar_t * Token::typeName() const {
        return typeName(m_type);
    }

    const cachar_t * Token::typeName(const Type & type) {
        return (type == Type::Eoi ? grammarInfo[0] : grammarInfo[int(type)]).name;
    }

    ostream & operator<<(ostream & stream, const Token & self) {
        typedef Token::Type Type;
        using namespace Cui;

        if (self.isKeyword()) {
            return stream << fgnd(2, 2, 5)
                          << ca('{')
                          << bold
                          << self.typeName()
                          << bold_off
                          << ca('}')
                          << reset;
        } else if (self.isPunctuation()) {
            return stream << fgnd(5, 2, 0)
                          << ca('{')
                          << bold
                          << self.typeName()
                          << bold_off
                          << ca('}')
                          << reset;
        }
        stream << ca('{') << bold << self.typeName();
        switch (self.type()) {
            case Type::Identifier:
            case Type::Number:
            case Type::String:{
                stream << ca(", ") << self.m_substr << bold_off;
                break;
            }
            default: {

            }
        }
        return stream << ca('}');
    }

    String & operator+=(String & string, const Token & self) {
        typedef Token::Type Type;
        string += ca("{");
        string += self.typeName();
        switch (self.type()) {
            case Type::Number:
            case Type::Identifier: {
                (string += ca(", ")) += self.m_substr;
                break;
            }
            case Type::String: {
                string.push_back(ca(", \""), self.m_substr, ca("\""));
                break;
            }
            default: break;
        }
        return string += ca('}');
    }

    lexertl::basic_rules<cachar_t, cachar_t> Token::makeLexertlRules() {
        using LtlRules = lexertl::basic_rules<cachar_t, cachar_t>;
        LtlRules rules;
        for (auto * iterator(grammarInfo + 1);
                    iterator != std::end(grammarInfo);
                  ++iterator){
            rules.push(
                iterator->regex,
                static_cast<LtlRules::id_type>(iterator->type)
            );
        }
        return rules;
    }
}