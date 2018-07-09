#include <locale>
#include <iostream>

#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <lexer.h>
#include <token.h>
#include "meta.h"
/* Convenient macro in order not to mention each test suite's name for a group of related tests,
 * just use: #define CALA_TEST_SUITE SuiteClass
 * to mark a suite name for the following group of tests
 * use
 * #undef CALA_TEST_SUITE
 * before redefining this macro for the next group of tests
*/
#define CALA_TEST(NAME) TEST_F(CALA_TEST_SUITE, NAME)

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


struct LexerSplitTokensTest : public ::testing::Test {
    typedef Calamity::Lexer     Lexer    ;
    typedef Calamity::String    String   ;
    typedef Calamity::Substring Substring;
    typedef Calamity::cachar_t  cachar_t ;
    typedef Calamity::Token     Token    ;
    Lexer lexer;
    String code;
    String returned;
    typedef Token::Type T;
    const String & split(const cachar_t * codeCstring){
        code = codeCstring;
        return returned = lexer.splitTokens(&code);
    }
    const String & split(String && str){
        code = std::move(str);
        return returned = lexer.splitTokens(&code);
    }

    const std::vector<Token> tokens() const {
        return lexer.tokens();
    }
    Substring subcode(const size_t & begin, const size_t & end){
        return Substring(code.begin() + begin, code.begin() + end);
    }
};

std::ostream & operator<<(std::ostream & stream, const Calamity::String & string){
    return stream << string.string();
}

// ==============================================================================================
// LexerSplitTokensTest test suite
#define CALA_TEST_SUITE LexerSplitTokensTest

#define token(TYPE, BEGIN, END) Token(TYPE, subcode((BEGIN), (END)))

#define splitOK_EXP(CODE)     ({ASSERT_TRUE (split(CODE).empty()) << returned;})
#define splitFAIL_EXP(CODE)   ({ASSERT_FALSE(split(CODE).empty()) << returned;})
#define splitOK(CODE)   splitOK_EXP  (ca(CODE))
#define splitFAIL(CODE) splitFAIL_EXP(ca(CODE))

#define matchTokens(TOKENS...) ({                            \
    const Token expected[]{TOKENS};                          \
    ASSERT_EQ(tokens().size(), Meta::sizeat(expected));      \
    for (size_t i = 0; i < Meta::sizeat(expected); ++i){     \
        ASSERT_TRUE(expected[i] == tokens()[i])              \
            << "failed at tokens()[" << i << ']';            \
    }})

#define expectTokensFor(CODE, TOKENS...) ({\
    splitOK(CODE);                         \
    matchTokens(TOKENS);                   \
})

CALA_TEST(emptyCode){
    splitOK("");
    ASSERT_TRUE(tokens().empty());
}

CALA_TEST(oneNumberToken){
   splitOK("13");
   ASSERT_EQ(tokens().size(), 1);
   //
   matchTokens(token(T::Number,0, 2));
}

CALA_TEST(oneEqualsToken){
   splitOK("==");
   matchTokens(token(T::Equality, 0, 2));
}

CALA_TEST(invalidChar){
   splitFAIL("№");
   splitFAIL("`");
    splitFAIL("#");
}

CALA_TEST(unterminatedMLComment){
    expectTokensFor("/* treatAsDivMult /*",
        token(T::Division, 0, 1),
        token(T::Multiplication, 1, 2),
        token(T::Identifier, 3, 17),
        token(T::Division, 18, 19),
        token(T::Multiplication, 19, 20)
    );
    splitOK("// /* is part of SLComment");
    ASSERT_TRUE(tokens().empty());
}

CALA_TEST(subsequentEqualSigns){
    expectTokensFor("=",   token(T::Assign,   0, 1));
    expectTokensFor("==",  token(T::Equality, 0, 2));
    expectTokensFor("===", token(T::Identity, 0, 3));
    expectTokensFor("====",
       token(T::Identity, 0, 3),
       token(T::Assign,   3, 4)
    );
    expectTokensFor("=====",
        token(T::Identity, 0, 3),
        token(T::Equality, 3, 5)
    );
    expectTokensFor("======",
        token(T::Identity, 0, 3),
        token(T::Identity, 3, 6)
    );
}

#define test_exp(CODE) ASSERT_TRUE(Macro_invoke(split(u##CODE).empty()))
#define test(CODE) test_exp(CODE)

CALA_TEST(unterminatedStrlit){
    splitFAIL("  \" unterminated literal  ");
    splitFAIL("\"");
    splitFAIL("\"\"\"");
    splitFAIL("\"\"\"\"\"");
    // apostrophe variant
    splitFAIL("  \' unterminated literal  ");
    splitFAIL("\'");
    splitFAIL("\'\'\'");
    splitFAIL("\'\'\'\'\'");
    //
    splitFAIL("\"no ending quote\'");
    splitFAIL("\'no ending apostrophe\"");
}

CALA_TEST(validStrlit){
    expectTokensFor("\"The string!\"", token(T::String, 1, 12));
    expectTokensFor("\'The string!\'", token(T::String, 1, 12));
    expectTokensFor("\"\"", token(T::String, 1, 1));
    expectTokensFor("\'\'", token(T::String, 1, 1));
    expectTokensFor("\" \' \" ", token(T::String, 1, 4));
    expectTokensFor("\' \" \' ", token(T::String, 1, 4));
    expectTokensFor("\" \\\"esc\\\" \"", token(T::String, 1, 10));
    expectTokensFor("\' \\\'esc\\\' \'", token(T::String, 1, 10));
    expectTokensFor("\"\n\"", token(T::String, 1, 2));
    expectTokensFor("\'\n\'", token(T::String, 1, 2));
}

CALA_TEST(validIdentifiers){
    expectTokensFor("idfier", token(T::Identifier, 0, 6));
    expectTokensFor("id id2 2id",
        token(T::Identifier, 0, 2),
        token(T::Identifier, 3, 6),
        token(T::Number,     7, 8),
        token(T::Identifier, 8, 10)
    );
// @todo non-latin idetifiers support
//    expectTokensFor("айдентифайер$ Її$Г",
//        token(T::Identifier, 0, 13),
//        token(T::Identifier, 14, 18)
//    );
    expectTokensFor("$", token(T::Identifier, 0, 1));
}

CALA_TEST(keywords){
    expectTokensFor("try",  token(T::Try, 0, 3));
    expectTokensFor("_try", token(T::Identifier, 0, 4));
    expectTokensFor("1else",
        token(T::Number, 0, 1),
        token(T::Else,   1, 5)
    );
    expectTokensFor("return$", token(T::Identifier, 0, 7));
    expectTokensFor("elseif",  token(T::Identifier, 0, 6));
    String suspectStr;
    for (int toktype = 1;
        Token::isKeyword(static_cast<Token::Type>(toktype));
        ++toktype){
        suspectStr = Token::typeName(static_cast<Token::Type>(toktype));
        suspectStr += ca("_");
        ASSERT_TRUE(split(std::move(suspectStr)).empty());
        matchTokens(token(T::Identifier, 0, code.size()));
    }
}



CALA_TEST(numberLiterals){
    expectTokensFor("23",    token(T::Number, 0, 2));
    expectTokensFor("23.23", token(T::Number, 0, 5));
    expectTokensFor(".312",  token(T::Number, 0, 4));
    expectTokensFor("12.34", token(T::Number, 0, 5));
    expectTokensFor("65.",
        token(T::Number, 0, 2),
        token(T::Dot,    2, 3)
    );
}

CALA_TEST(punctuation){
    expectTokensFor("{}",
        token(T::LeftCurly, 0, 1),
        token(T::RightCurly, 1, 2)
    );
    expectTokensFor(",:][;",
        token(T::Coma, 0, 1),
        token(T::Colon, 1, 2),
        token(T::RightBracket, 2, 3),
        token(T::LeftBracket, 3, 4),
        token(T::Semicolon, 4, 5)
    );
}


CALA_TEST(operators) {
    expectTokensFor(">>=<=+()",
        token(T::Bit2RShiftAssign, 0, 3),
        token(T::LessOrEqual, 3, 5),
        token(T::Plus, 5, 6),
        token(T::LeftPar, 6, 7),
        token(T::RightPar, 7, 8)
    );
    expectTokensFor("=>=",
        token(T::Arrow, 0, 2),
        token(T::Assign, 2, 3)
    );
    expectTokensFor("^ = ^=",
        token(T::BitXor, 0, 1),
        token(T::Assign, 2, 3),
        token(T::BitXorAssign, 4, 6)
    );
    expectTokensFor("*****/%---+++",
        token(T::Power, 0, 2),
        token(T::Power, 2, 4),
        token(T::Multiplication, 4, 5),
        token(T::Division, 5, 6),
        token(T::Remainder, 6, 7),
        token(T::Decrement, 7, 9),
        token(T::Minus, 9, 10),
        token(T::Increment, 10, 12),
        token(T::Plus, 12, 13)
    );
}

CALA_TEST(assignmentOperators){
    expectTokensFor("+=-=*=**=%=/=",
        token(T::PlusAssign, 0, 2),
        token(T::MinusAssign, 2, 4),
        token(T::MultiplyAssign, 4, 6),
        token(T::PowerAssign, 6, 9),
        token(T::RemainderAssign, 9, 11),
        token(T::DivideAssign, 11, 13)
    );
}
