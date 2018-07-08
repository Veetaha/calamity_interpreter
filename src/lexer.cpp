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
            ca("unexpected token ["), *tokenIt , ca("] at ("), mistakePoint.y,
                                                    ca(" : "), mistakePoint.x, ca(")\n"),
            mistakePoint.y, ca(". ["), m_code->lineOf(tokenIt), ca(']')
        );
    }

    String Lexer::splitTokens(const String * const & code) {
        namespace ltl = lexertl;
        using Cui::newline;
        if (!code){
            return ca("code == nullptr");
        }
        m_code = code;
        m_tokens.clear();
        ltl::match_results<String::const_iterator> match(code->begin(), code->end());

        for (ltlgen::lookup(match); match.id; ltlgen::lookup(match)) {
            if (match.id == ltl::u16smatch::npos()){
                return unexpectedTokenException(match.first);
            }

            if (std_ext::equalsOneOf(match.id,
                static_cast<decltype(match.id)>(Token::Type::Newline),
                static_cast<decltype(match.id)>(Token::Type::MLComment))) {
                // todo or not todo: ignoring newlines and multiline comments
            } else if (match.id == static_cast<decltype(match.id)>(Token::Type::String)){
                // flatten the string to omit leading and trailing quotes or apostrophes
                m_tokens.emplace_back(
                    Token::Type::String,
                    match.first + 1,
                    match.second - 1
                );
            } else {
                m_tokens.emplace_back(
                    static_cast<Token::Type>(match.id),
                    match.first,
                    match.second
                );
            }
        }
        return ca("");
    }

    ostream & operator<<(ostream & stream, const Lexer & self) {
        typedef Token::Type Type;
        for (const Token & token : self.tokens()){
            stream << token
                   << ((token.hasType(Type::Semicolon)
                     || token.hasType(Type::RightCurly))
                       ? L'\n'
                       : L' '
                   );
        }
        return stream;
    }
}