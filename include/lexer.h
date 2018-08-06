#pragma once

#include <vector>
#include <iostream>
#include <memory>

#include "token.h"
#include "var_string.h"
#include "native_exception.h"
#include "not_null_ptr.h"
#include "defs.h"

namespace Cala {




	class LexicalInfo {
	friend class GrammaticalInfo;
	    std::vector<Token> m_tokens;
		std::shared_ptr<String> m_code;
    public:
		DECL_DEFAULT_MOVE_ONLY(LexicalInfo)

		typedef std::vector<Token>::const_iterator const_iterator;

	    explicit LexicalInfo(const String & code)
	        : m_code(std::make_shared<String>(code)) {}

        explicit LexicalInfo(String && code)
            : m_code(std::make_shared<String>(std::move(code))) {}

        /**
         * @brief Constructs a LexicalInfo object and takes an ownership of code pointer.
         *
         */
        explicit LexicalInfo(const std::shared_ptr<String> & code)
            : m_code(code) {}

        explicit LexicalInfo(std::shared_ptr<String> && code)
            : m_code(std::move(code)) {}


        LexicalInfo() = default;


        const std::vector<Token> & tokens() && = delete;
        const String & code() && = delete;


        /**
         * @return Const reference to a string containing source code.
         */
        inline const std::shared_ptr<String> & code() const &   { return m_code; }
        inline std::shared_ptr<String> & code() &               { return m_code; }

        inline const std::vector<Token> & tokens() const & { return m_tokens; }
        inline std::vector<Token> & tokens()             & { return m_tokens; }


        friend ostream & operator<<(ostream & stream, const LexicalInfo & self);

        Exception unexpectedTokenException(String::const_iterator & tokenIt);
    };


    namespace Lexer {
        /**
         * @brief Tokenize a string of code written in Calamity programming language.
         * @param code  String containing actual code for lexical analysis.
         * @return  LexicalInfo object containing dynamically allocated copy of the code and
         * linear std::vector<Token> sequence representing the results of the analysis.
         *
         * @throws Cala::Exception if any kind of lexical mistake was detected.
         */

        LexicalInfo splitTokens(const String & code);

        /**
         * @brief Same as splitTokens(), but reads the code from the specified file.
         * @param filePath  Absolute or relation path to the file containing Calamity program source code.
         *
         * @throws Cala::Exception if any kind of lexical mistake was deteced or if
         * couldn't create and open a file handle to read input from.
         */
        LexicalInfo splitTokensFromFile(const char * const & filePath);
    }

}
