#include <iostream>
#include <lexer.h>
#include <error.h>
#include <token.h>
#include <cui.h>
#include <lexertl/match_results.hpp>
#include <boost/range.hpp>
#include "lexertl_gen.h"

namespace Calamity {

    String Lexer::unexpectedTokenException(String::const_iterator & tokenIt){
        Point2D<size_t> mistakePoint(m_code->point2DOf(tokenIt));
        return String().push_back(
            u"unexpected token [", *tokenIt ,u"] at (", mistakePoint.y, u" : ", mistakePoint.x, u")\n",
            mistakePoint.y, u". [", m_code->lineOf(tokenIt), u']'
        );
    }

    String Lexer::splitTokens(const String * const & code) {
        namespace ltl = lexertl;
        using Cui::newline;
        if (!code){
            return u"code == nullptr";
        }
        m_code = code;
        ltl::u16smatch match(code->begin(), code->end());

        std::wcout << Cui::bold << L"Starting tokenization...\n" << Cui::bold_off;

        for (ltlgen::lookup(match); match.id; ltlgen::lookup(match)) {
            if (match.id == ltl::u16smatch::npos()){
                return unexpectedTokenException(match.first);
            }

            if (std_ext::equalsOneOf(match.id,
                static_cast<decltype(match.id)>(Token::Type::Newline),
                static_cast<decltype(match.id)>(Token::Type::MLComment))) {
                std::wcout << newline;
//            } else if (match.id == static_cast<decltype(match.id)>(Token::Type::MLComment){
//                Substring mlcSubstr(match.first, match.second);
//                if (mlcSubstr.contains('\n')) {
//                    m_tokens.emplace_back(Token::Type::Newline, std::move(mlcSubstr));
//                }
//                std::wcout << newline;
            } else {
                m_tokens.emplace_back(
                    static_cast<Token::Type>(match.id),
                    std::move(match.first),
                    std::move(match.second)
                );
                std::wcout << m_tokens.back() << L" ";
            }
        }

        std::wcout << Cui::bold << L"\nFinished tokenization\n" << Cui::bold_off;

        return u"";
    }

    std::wostream & operator<<(std::wostream & stream, const Lexer & self) {
        for (const Token & token : self.tokens()){
            stream << token << (token.isSemicolon() ? L'\n' : L' ');
        }
        return stream;
    }
}