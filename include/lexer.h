#pragma once

#include <token.h>
#include <vector>
#include <iostream>
#include "var_string.h"

namespace Calamity {

	class Lexer {
        std::vector<Token> m_tokens;
        const String * m_code;

    public:
        Lexer() = default;

        String splitTokens(const String * const & code);

        inline const std::vector<Token> & tokens() const { return m_tokens; }
        inline std::vector<Token> & tokens() { return m_tokens; }

        inline const String * const & code() const { return m_code; }

        friend std::wostream & operator<<(std::wostream & stream, const Lexer & self);

    private:
        String unexpectedTokenException(String::const_iterator & tokenIt);
    };
}