#include <iostream>
#include <lexertl/match_results.hpp>
#include <boost/range.hpp>

#include "lexer.h"
#include "token.h"
#include "cui.h"
#include "lexertl_gen.h"
#include "native_exception.h"

namespace Cala {

    Exception LexicalInfo::unexpectedTokenException(String::const_iterator & tokenIt){
        Point2D<size_t> mistakePoint(m_code->point2DOf(tokenIt));
        String exceptionString;
        return Exception(
            std::move(
                exceptionString.push_back(
                    ca("unexpected token ["), *tokenIt,
                    ca("] at ("), mistakePoint.y, ca(" : "), mistakePoint.x, ca(")\n"),
                    mistakePoint.y, ca(". ["), m_code->lineOf(tokenIt), ca(']')
                )
            )
        );
    }

    namespace Lexer {
        LexicalInfo splitTokens(const String & code) {
            LexicalInfo info(code);
            namespace ltl = lexertl;
            typedef ltl::match_results<String::const_iterator> ltl_match_results;

            ltl_match_results match(info.code()->cbegin(), info.code()->cend());

            for (ltlgen::lookup(match); match.id; ltlgen::lookup(match)) {
                // No such token was found
                if (match.id == ltl_match_results::npos()) {
                    throw info.unexpectedTokenException(match.first);
                }

                typedef decltype(match.id) ltl_id_type;
                typedef Token::Type Type;

                if (Vtem::equalsOneOf(
                    match.id,
                    static_cast<ltl_id_type>(Type::Newline),
                    static_cast<ltl_id_type>(Type::MLComment))) {
                    // todo or not todo: ignoring newlines and multiline comments
                } else if (match.id == static_cast<ltl_id_type>(Type::String)) {
                    // flatten the string to omit leading and trailing quotes or apostrophes
                    info.tokens().emplace_back(
                        Type::String,
                        match.first + 1,
                        match.second - 1
                    );
                } else {
                    info.tokens().emplace_back(
                        static_cast<Token::Type>(match.id),
                        match.first,
                        match.second
                    );
                }
            }
            return info;
        }

        LexicalInfo splitTokensFromFile(const char * const & filePath){
            return splitTokens(String::readFromFile(filePath));
        }
    }

    ostream & operator<<(ostream & stream, const LexicalInfo & self) {
        typedef Token::Type Type;
        for (const Token & token : self.tokens()) {
            stream << token
                   << ((token.is(Type::Semicolon) || token.is(Type::RightCurly))
                       ? ca('\n')
                       : ca(' ')
                   );
        }
        return stream;
    }



}
