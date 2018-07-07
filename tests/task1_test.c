#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include <check.h>
#include <list.h>
#include <trie.h>
#include <string.h>

#include <lexer.h>
#include <token.h>
#include <floatingpoint.h>
#include <character.h>
#include <sstring.h>

void destr(void * ptr) {return;}


START_TEST (Lexer_splitTokens_empty_empty)
{
   List * tokens = List_new();
   int status = Lexer_splitTokens("", tokens);
   ck_assert_int_eq(status, 0);
   ck_assert_int_eq(List_size(tokens), 0);
   List_free(tokens);
}
END_TEST

START_TEST (Lexer_splitTokens_oneNumber_oneNumberToken)
{
   List * tokens = List_new();
   int status = Lexer_splitTokens("13", tokens);
   ck_assert_int_eq(status, 0);
   ck_assert_int_eq(List_size(tokens), 1);
   Token * firstToken = (Token *)List_at(tokens, 0);
   Token testToken = {.lex = &(Lexeme){LexemeType_FLOAT, String_toNewString("13")},
                       .loc = loc(1, 1)};   
   //
   ck_assert(Token_equals(firstToken, &testToken));
   String_free(testToken.lex->name);
   Lexer_clearTokens(tokens);
   List_free(tokens);
}
END_TEST


START_TEST (Lexer_splitTokens_oneEqualityToken_oneEqualsToken)
{
   List * tokens = List_new();
   int status = Lexer_splitTokens("==", tokens);
   ck_assert_int_eq(status, 0);
   ck_assert_int_eq(List_size(tokens), 1);
   Token * firstToken = (Token *)List_at(tokens, 0);
   Token testToken = { .lex = &(Lexeme){LexemeType_EQ, NULL}, .loc = loc(1, 1)};
    
   ck_assert(Token_equals(firstToken, &testToken));
   Lexer_clearTokens(tokens);
   List_free(tokens);
}
END_TEST

START_TEST (Lexer_splitTokens_invalidChar_errorCode)
{
   List * tokens = List_new();
   int status = Lexer_splitTokens("&", tokens);
   ck_assert_int_eq(status, LEXER_ERROR_UNHANDLED_TOKEN);
   ck_assert_int_eq(List_size(tokens), 0);
   List_free(tokens);
}
END_TEST


START_TEST (Lexer_splitTokens_twoEqualityToken_twoEqualityTokens){
    List * tokens = List_new();
   int status = Lexer_splitTokens("====", tokens);
   ck_assert_int_eq(status, 0);
   ck_assert_int_eq(List_size(tokens), 2);
   Token * firstToken = (Token *)List_at(tokens, 0);
   Token testToken = (Token){ .lex = &(Lexeme){LexemeType_EQ, NULL}, .loc = loc(1, 1)};
   ck_assert(Token_equals(firstToken, &testToken));
   //
   firstToken = List_at(tokens, 1);
   testToken.loc = loc(1, 3);
   ck_assert(Token_equals(firstToken, &testToken));
   Lexer_clearTokens(tokens);
   List_free(tokens);
} END_TEST

START_TEST (Lexer_splitTokens_unterminatedCharLiteral_errorCode){
    List * tokens = List_new();
    int status = Lexer_splitTokens("'c", tokens);
    ck_assert_int_eq(status, LEXER_ERROR_UNTERMINATED_LITERAL);
    ck_assert_int_eq(List_size(tokens), 0);
    //
    status = Lexer_splitTokens(" 'c ' ", tokens);
    ck_assert_int_eq(status, LEXER_ERROR_UNTERMINATED_LITERAL);
    ck_assert_int_eq(List_size(tokens), 0);
    List_free(tokens);
} END_TEST

START_TEST (Lexer_splitTokens_unterminatedStringLiteral_errorCode){
    List * tokens = List_new();
    int status = Lexer_splitTokens("\" wa", tokens);
    ck_assert_int_eq(status, LEXER_ERROR_UNTERMINATED_LITERAL);
    ck_assert_int_eq(List_size(tokens), 0);
    //
    status = Lexer_splitTokens("\"", tokens);
    ck_assert_int_eq(status, LEXER_ERROR_UNTERMINATED_LITERAL);
    ck_assert_int_eq(List_size(tokens), 0);
    //
    status = Lexer_splitTokens("\"\"\"", tokens);
    ck_assert_int_eq(status, LEXER_ERROR_UNTERMINATED_LITERAL);
    ck_assert_int_eq(List_size(tokens), 0);
    List_free(tokens);
} END_TEST

START_TEST (Lexer_splitTokens_invalidFloatingPointLiteral_errorCode){
    List * tokens = List_new();
    int status = Lexer_splitTokens("23.", tokens);
    ck_assert_int_eq(status, LEXER_ERROR_UNTERMINATED_LITERAL);
    ck_assert_int_eq(List_size(tokens), 0);
    //
    status = Lexer_splitTokens("23.23.", tokens);
    ck_assert_int_eq(status, LEXER_ERROR_UNHANDLED_TOKEN);
    ck_assert_int_eq(List_size(tokens), 0);
    List_free(tokens);
} END_TEST

START_TEST (Lexer_splitTokens_oneKeyword_oneKeywordToken){
    List * tokens = List_new();
    int status = Lexer_splitTokens("else", tokens);
    ck_assert_int_eq(status, 0);
    ck_assert_int_eq(List_size(tokens), 1);
    Token * firstToken = (Token *)List_at(tokens, 0);
    Token testToken = { .lex = &(Lexeme){LexemeType_OTHERWISE, NULL}, .loc = loc(1, 1)};
    ck_assert(Token_equals(firstToken, &testToken));
    Lexer_clearTokens(tokens);
    List_free(tokens);
} END_TEST

START_TEST (Lexer_splitTokens_oneIdentifier_oneIdentifierToken){
    List * tokens = List_new();
    int status = Lexer_splitTokens("_IDENTIFIER_____", tokens);
    ck_assert_int_eq(status, 0);
    ck_assert_int_eq(List_size(tokens), 1);
    Token * firstToken = (Token *)List_at(tokens, 0);
    Token testToken = { .lex = &(Lexeme){LexemeType_IDENTIFIER, String_toNewString("_IDENTIFIER_____")},
                         .loc = loc(1, 1)};
    ck_assert(Token_equals(firstToken, &testToken));
    Lexer_clearTokens(tokens);
    List_free(tokens);
    String_free(testToken.lex->name);
} END_TEST

START_TEST (Lexer_splitTokens_validEspaceSequenceCharacterLiteral_oneCharToken){
    List * tokens = List_new();
    int status = Lexer_splitTokens("'\\n'", tokens);
    ck_assert_int_eq(status, 0);
    ck_assert_int_eq(List_size(tokens), 1);
    Token * firstToken = (Token *)List_at(tokens, 0);
    Token testToken = { .lex = 
                        &(Lexeme){
                            LexemeType_CHARACTER, 
                            String_toNewString("\n")
                        },
                        .loc = loc(1, 1)
                    };
    ck_assert(Token_equals(firstToken, &testToken));
    Lexer_clearTokens(tokens);
    List_free(tokens);
    String_free(testToken.lex->name);
} END_TEST

START_TEST (Lexer_splitTokens_validEspaceSequenceInStringLiteral_onePowToken){
    List * tokens = List_new();
    int status = Lexer_splitTokens(" \"_34\tsda\"", tokens);
    ck_assert_int_eq(status, 0);
    ck_assert_int_eq(List_size(tokens), 1);
    Token * firstToken = (Token *)List_at(tokens, 0);
    Token testToken = { .lex = &(Lexeme){LexemeType_STRING, String_toNewString("_34\tsda")},
                         .loc = loc(1, 2)};

    ck_assert(Token_equals(firstToken, &testToken));
    Lexer_clearTokens(tokens);
    List_free(tokens);
    String_free(testToken.lex->name);
} END_TEST

START_TEST (Lexer_splitTokens_invalidEspaceSequenceCharacterLiteral_errorCode){
    List * tokens = List_new();
    int status = Lexer_splitTokens("'\\", tokens);
    ck_assert_int_eq(status, LEXER_ERROR_UNTERMINATED_LITERAL);
    ck_assert_int_eq(List_size(tokens), 0);
    //
    status = Lexer_splitTokens("'\\5", tokens);
    ck_assert_int_eq(status, LEXER_ERROR_UNHANDLED_TOKEN);
    ck_assert_int_eq(List_size(tokens), 0);
    //
    status = Lexer_splitTokens("'\\g'", tokens);
    ck_assert_int_eq(status, LEXER_ERROR_UNHANDLED_TOKEN);
    ck_assert_int_eq(List_size(tokens), 0);
    List_free(tokens);
} END_TEST

START_TEST (Lexer_splitTokens_invalidEspaceSequenceStringLiteral_errorCode){
    List * tokens = List_new();
    int status = Lexer_splitTokens("\"\\\"", tokens);
    ck_assert_int_eq(status, LEXER_ERROR_UNTERMINATED_LITERAL);
    ck_assert_int_eq(List_size(tokens), 0);
    //
    status = Lexer_splitTokens("\"\\_\"", tokens);
    ck_assert_int_eq(status, LEXER_ERROR_UNHANDLED_TOKEN);
    ck_assert_int_eq(List_size(tokens), 0);
    //
    status = Lexer_splitTokens("'\\g'", tokens);
    ck_assert_int_eq(status, LEXER_ERROR_UNHANDLED_TOKEN);
    ck_assert_int_eq(List_size(tokens), 0);
    List_free(tokens);
} END_TEST

START_TEST (Lexer_splitTokens_emptyStringLiteral_oneLiteralToken){
    List * tokens = List_new();
    int status = Lexer_splitTokens("\"\"", tokens);
    ck_assert_int_eq(status, 0);
    ck_assert_int_eq(List_size(tokens), 1);
    Token * firstToken = (Token *)List_at(tokens, 0);
    Token testToken = { .lex = &(Lexeme){LexemeType_STRING, String_toNewString("")},
                         .loc = loc(1, 1)};
    // String_append(DynamicType_valAt(testToken.val), "");
    ck_assert(Token_equals(firstToken, &testToken));
    Lexer_clearTokens(tokens);
    List_free(tokens);
    String_free(testToken.lex->name);
} END_TEST

























START_TEST (List_isEmpty_newList_isTrue)
{
    List * test = List_new();
    ck_assert_int_eq(List_isEmpty(test), true);
    List_free(test);
}
END_TEST

START_TEST (List_isEmpty_oneElement_isFalse)
{
    List * test = List_new();
    List_insert(test, 0, (void *)2);
    ck_assert_int_eq(List_isEmpty(test), false);
    ck_assert(List_at(test, 0) == (void *)2);
    ck_assert(List_removeAt(test, 0) == (void *)2);
    List_free(test);
}
END_TEST

START_TEST (List_isEmpty_onePushedOnePopped_isTrue)
{
    List * test = List_new();
    List_insert(test, 0, (void *)2);
    List_removeAt(test, 0);
    ck_assert_int_eq(List_isEmpty(test), true);
    List_free(test);
}
END_TEST

// TRIE ---------------------------------------------------------------
START_TEST (Trie_insert_emtyTrie_isTrue){
    Trie * test = Trie_new();
    ck_assert_int_eq(Trie_insert(test, "Testuvannya", NULL), true);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_insert_hasOneUniqueWord_isTrue){
    Trie * test = Trie_new();
    Trie_insert(test, "-_@#awdxc", NULL);
    ck_assert_int_eq(Trie_insert(test, "Unique word", NULL), true);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_insert_hasOneWordWithCommonLetter_isTrue){
    Trie * test = Trie_new();
    Trie_insert(test, "-_@#awdxc", NULL);
    ck_assert_int_eq(Trie_insert(test, "-_sdasdwad", NULL), true);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_insert_hasTwoWordsWithCommonLetter_isTrue){
    Trie * test = Trie_new();

    Trie_insert(test, "#$awdxc", NULL);
    ck_assert_int_eq(Trie_insert(test, "#$sdasdwad", NULL), true);
    ck_assert_int_eq(Trie_insert(test, "#$aaaadxc", NULL), true);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_insert_hasSameWord_isFalse){
    Trie * test = Trie_new();
    Trie_insert(test, "213", NULL);

    ck_assert_int_eq(Trie_insert(test, "213", NULL), false);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_insert_zeroLengthString_isTrue){
    Trie * test = Trie_new();

    ck_assert_int_eq(Trie_insert(test, "", NULL), true);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_insert_zeroLengthStringTwice_isFalse){
    Trie * test = Trie_new();
    Trie_insert(test, "", NULL);
    ck_assert_int_eq(Trie_insert(test, "", NULL), false);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_insert_hasMultipleWordsWithCommonLeter_isTrue){
    Trie * test = Trie_new();
    ck_assert_int_eq(Trie_insert(test, "ABCD", NULL), true);
    ck_assert_int_eq(Trie_insert(test, "ABDCV", NULL), true);
    ck_assert_int_eq(Trie_insert(test, "ABTTRR", NULL), true);
    ck_assert_int_eq(Trie_insert(test, "ABCDFVC", NULL), true);
    ck_assert_int_eq(Trie_insert(test, "ABTTRS", NULL), true);
    ck_assert_int_eq(Trie_insert(test, "ABDCCV", NULL), true);
    ck_assert_int_eq(Trie_insert(test, "VBAAA", NULL), true);
    ck_assert_int_eq(Trie_insert(test, "ASDWD", NULL), true);
    ck_assert_int_eq(Trie_insert(test, "SSSS", NULL), true);
    ck_assert_int_eq(Trie_insert(test, "", NULL), true);
    ck_assert_int_eq(Trie_insert(test, "SDAS2@#", NULL), true);
    ck_assert_int_eq(Trie_insert(test, "@#$#Q@QWDSDA", NULL), true);
    ck_assert_int_eq(Trie_insert(test, "ABCDFVC", NULL), false);
    Trie_free(test, (Destructor)destr);
} END_TEST


START_TEST (Trie_insert_isPartOfAPresentWord_isTrue){
    Trie * test = Trie_new();
    Trie_insert(test, "ABCD", NULL);
    ck_assert_int_eq(Trie_insert(test, "ABC", NULL), true);
    ck_assert_int_eq(Trie_insert(test, "ABDRT", NULL), true);
    ck_assert_int_eq(Trie_insert(test, "ABDR", NULL), true);
    Trie_free(test, (Destructor)destr);
} END_TEST


START_TEST (Trie_search_hasOneWord_isTrue){
    Trie * test = Trie_new();
    bool status = FAILURE;
    Trie_insert(test, "KEY", (void*) 1);
    ck_assert_ptr_eq(Trie_search(test, "KEY", &status), (void *)1);
    ck_assert_int_eq(status, SUCCESS);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_search_hasTwoUniqueWords_isTrue){
    Trie * test = Trie_new();
    bool status = FAILURE;
    Trie_insert(test, "KEY", (void*) 1);
    Trie_insert(test, "AND", (void*) 2);
    ck_assert_ptr_eq(Trie_search(test, "KEY", &status), (void *)1);
    ck_assert_int_eq(status, SUCCESS);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_search_hasTwoWordsWithCommonLetter_isTrue){
    Trie * test = Trie_new();
    bool status = FAILURE;
    Trie_insert(test, "KEYED", (void*) 1);
    Trie_insert(test, "KEYOD", (void*) 2);
    ck_assert_ptr_eq(Trie_search(test, "KEYOD", &status), (void *)2);
    ck_assert_int_eq(status, SUCCESS);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_search_hasMultipleWordsWithCommonLetter_isTrue){
    Trie * test = Trie_new();
    bool status = FAILURE;
    Trie_insert(test, "ABCD", (void *)1);
    Trie_insert(test, "ABDCV", (void *)2);
    Trie_insert(test, "ABTTRR", (void *)3);
    Trie_insert(test, "ABCDFVC", (void *)4);
    Trie_insert(test, "ABTTRS", (void *)5);
    Trie_insert(test, "ABDCCV", (void *)6);
    Trie_insert(test, "VBAAA", (void *)7);
    Trie_insert(test, "ASDWD", (void *)8);
    Trie_insert(test, "SSSS", (void *)9);
    Trie_insert(test, "", (void *)10);
    Trie_insert(test, "SDAS2@#", (void *)11);
    Trie_insert(test, "@#$#Q@QWDSDA", (void *)12);
    Trie_insert(test, "@#$#3213@QWDSDA", (void *)13);
    Trie_insert(test, "@#$#Q@241DSDA", (void *)14);
    Trie_insert(test, "@#$#Q@QtfrgdWDSDA", (void *)15);
    Trie_insert(test, "@#$#Q@QWDdgfDA", (void *)16);
    Trie_insert(test, "ABCDFVC", (void *)13);

    ck_assert_ptr_eq(Trie_search(test, "@#$#Q@QWDSDA", &status), (void *)12);
    ck_assert_int_eq(status, SUCCESS);

    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_search_hasNoWord_isFalse){
    Trie * test = Trie_new();
    bool status = true;
    Trie_insert(test, "ABCD", (void *)1);
    Trie_insert(test, "ABDCV", (void *)2);
    Trie_insert(test, "ABTTRR", (void *)3);
    Trie_insert(test, "ABCDFVC", (void *)4);
    Trie_insert(test, "ABTTRS", (void *)5);
    Trie_insert(test, "ABDCCV", (void *)6);
    Trie_insert(test, "VBAAA", (void *)7);
    Trie_insert(test, "ASDWD", (void *)8);
    Trie_insert(test, "SSSS", (void *)9);
    Trie_insert(test, "", (void *)10);
    Trie_insert(test, "SDAS2@#", (void *)11);
    Trie_insert(test, "@#$#Q@QWDSDA", (void *)12);
    Trie_insert(test, "ABCDFVC", (void *)13);

    ck_assert_ptr_eq(Trie_search(test, "NO", &status), NULL);
    ck_assert_int_eq(status, FAILURE);

    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_search_hasOnlyAlikeWord_isFalse){
    Trie * test = Trie_new();
    bool status = true;
    Trie_insert(test, "ABCD", (void *)1);
    Trie_insert(test, "ABDCV", (void *)2);
    Trie_insert(test, "ABTTRR", (void *)3);
    Trie_insert(test, "ABCDFVC", (void *)4);
    Trie_insert(test, "ABTTRS", (void *)5);
    Trie_insert(test, "ABDCCV", (void *)6);
    Trie_insert(test, "VBAAA", (void *)7);
    Trie_insert(test, "ASDWD", (void *)8);
    Trie_insert(test, "SSSS", (void *)9);
    Trie_insert(test, "", (void *)10);
    Trie_insert(test, "SDAS2@#", (void *)11);
    Trie_insert(test, "@#$#Q@QWDSDA", (void *)12);
    Trie_insert(test, "ABCDFVC", (void *)13);

    ck_assert_ptr_eq(Trie_search(test, "ABCD NO", &status), NULL);
    ck_assert_int_eq(status, FAILURE);

    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_search_trieIsEmpty_isFalse){
    Trie * test = Trie_new();
    bool status = true;
    ck_assert_ptr_eq(Trie_search(test, "NOTHING", &status), NULL);
    ck_assert_int_eq(status, FAILURE);

    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_remove_hasOneWord_validPtr){
    Trie * test = Trie_new();
    bool status = FAILURE;
    Trie_insert(test, "NOTHING", (void *)23);
    ck_assert_ptr_eq(Trie_remove(test, "NOTHING", &status), (void *)23);
    ck_assert_int_eq(status, SUCCESS);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_remove_hasTwoWordsWithCommonLetter_validPtr){
    Trie * test = Trie_new();
    bool status = FAILURE;
    Trie_insert(test, "NOTHING", (void *)234);
    Trie_insert(test, "NOT", (void *)23);
    ck_assert_ptr_eq(Trie_remove(test, "NOTHING", &status), (void *)234);
    ck_assert_int_eq(status, SUCCESS);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_remove_isPartOfAPresentWord_validPtr){
    Trie * test = Trie_new();
    bool status = FAILURE;
    Trie_insert(test, "NOTHING", (void *)23);
    Trie_insert(test, "NOT", (void *)245);
    ck_assert_ptr_eq(Trie_remove(test, "NOT", &status), (void *)245);
    ck_assert_int_eq(status, SUCCESS);
    Trie_insert(test, "NOTR", (void *)223);
    Trie_insert(test, "NOTRER", (void *)2233);
    ck_assert_ptr_eq(Trie_remove(test, "NOTRER", &status), (void *)2233);
    ck_assert_int_eq(status, SUCCESS);
    
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_remove_hasMultipleWordsWithCommonLetter_validPtr){
    Trie * test = Trie_new();
    bool status = FAILURE;
    Trie_insert(test, "ABCD", (void *)1);
    Trie_insert(test, "ABDCV", (void *)2);
    Trie_insert(test, "ABTTRR", (void *)3);
    Trie_insert(test, "ABCDFVC", (void *)4);
    Trie_insert(test, "ABTTRS", (void *)5);
    Trie_insert(test, "ABDCCV", (void *)6);
    Trie_insert(test, "VBAAA", (void *)7);
    Trie_insert(test, "ASDWD", (void *)8);
    Trie_insert(test, "SSSS", (void *)9);
    Trie_insert(test, "", (void *)10);
    Trie_insert(test, "SDAS2@#", (void *)11);
    Trie_insert(test, "@#$#Q@QWDSDA", (void *)12);

    ck_assert_ptr_eq(Trie_remove(test, "ABCDFVC", &status), (void *)4);
    ck_assert_int_eq(status, SUCCESS);

    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_remove_emptyString_validPtr){
    Trie * test = Trie_new();
    bool status = FAILURE;
    Trie_insert(test, "ABCD", (void *)1);
    Trie_insert(test, "", (void*)23);

    ck_assert_ptr_eq(Trie_remove(test, "", &status), (void *)23);
    ck_assert_int_eq(status, SUCCESS);

    Trie_free(test, (Destructor)destr);
} END_TEST


START_TEST (Trie_isEmpty_newTrie_isTrue) {

    Trie * test = Trie_new();

    ck_assert_int_eq(Trie_isEmpty(test), true);

    Trie_free(test, (Destructor)destr);

} END_TEST

START_TEST (Trie_isEmpty_trieWithOneWord_isFalse) {
    Trie * test = Trie_new();
    Trie_insert(test, "Word", NULL);
    ck_assert_int_eq(Trie_isEmpty(test), false);

    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_isEmpty_trieWithEmptyString_isFalse) {
    Trie * test = Trie_new();
    Trie_insert(test, "", NULL);
    ck_assert_int_eq(Trie_isEmpty(test), false);
    bool status = FAILURE;
    Trie_remove(test, "", &status);
    ck_assert_int_eq(Trie_isEmpty(test), true);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_isEmpty_trieWithTwoWords_isFalse) {
    Trie * test = Trie_new();
    Trie_insert(test, "Word", NULL);
    Trie_insert(test, "Wordod", NULL);
    ck_assert_int_eq(Trie_isEmpty(test), false);

    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_isEmpty_trieInsertedAndRemovedWord_isTrue) {
    Trie * test = Trie_new();
    Trie_insert(test, "Word", NULL);
    bool status = FAILURE;
    Trie_remove(test, "Word", &status);
    ck_assert_int_eq(Trie_isEmpty(test), true);

    Trie_free(test, (Destructor)destr);
} END_TEST


START_TEST (Trie_remove_emptyTrie_nullFalse){
    Trie * test = Trie_new();
    bool status = true;
    ck_assert_ptr_eq(Trie_remove(test, "String", &status), NULL);
    ck_assert_int_eq(status, FAILURE);

    status = true;
    ck_assert_ptr_eq(Trie_remove(test, "", &status), NULL);
    ck_assert_int_eq(status, FAILURE);

    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_remove_hasNoSuchWord_nullFalse){
    Trie * test = Trie_new();
    Trie_insert(test, "WOR@#D", NULL);
    Trie_insert(test, "WOR2D", NULL);
    Trie_insert(test, "123ORD", NULL);
    Trie_insert(test, "W$#ORD", NULL);
    Trie_insert(test, "W@#ORD", NULL);
    Trie_insert(test, "WOR#$sD", NULL);
    Trie_insert(test, "WO$#RD", NULL);

    bool status = true;

    ck_assert_ptr_eq(Trie_remove(test, "WORD", &status), NULL);
    ck_assert_int_eq(status, FAILURE);

    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_remove_hasNoSuchWordWhichIsAPartOfAnother_nullFalse){
    Trie * test = Trie_new();
    Trie_insert(test, "WOR@#D", NULL);
    Trie_insert(test, "WORDLESS", NULL);
    Trie_insert(test, "WORDOK", NULL);
    Trie_insert(test, "W$#ORD", NULL);
    Trie_insert(test, "W@#ORD", NULL);
    Trie_insert(test, "WOR#$sD", NULL);
    Trie_insert(test, "WO$#RD", NULL);

    bool status = true;

    ck_assert_ptr_eq(Trie_remove(test, "WORD", &status), NULL);
    ck_assert_int_eq(status, FAILURE);

    Trie_free(test, (Destructor)destr);
} END_TEST


START_TEST (Trie_query_trieIsEmpty_isNULL){
    Trie * test = Trie_new();
    const char * keyEnd = (char *)23;
    ck_assert_ptr_eq(Trie_query(test, "WORD", &keyEnd), NULL);
    ck_assert_ptr_eq(keyEnd, NULL);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_query_trieHasOneValidWord_validPtr){
    Trie * test = Trie_new();
    const char * keyEnd = (char *)23;
    const char * str = "WORD";
    Trie_insert(test, str, (void *)24);
    ck_assert_ptr_eq(Trie_query(test, str, &keyEnd), (void*)24);
    ck_assert_ptr_eq(keyEnd, str + strlen(str) - 1);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_query_trieHasTwoWordsWithCommonLetter_validPtr){
    Trie * test = Trie_new();
    const char * keyEnd = (char *)23;
    const char * str = "WORD";
    Trie_insert(test, str, (void *)24);
    Trie_insert(test, "WORK", NULL);
    ck_assert_ptr_eq(Trie_query(test, str, &keyEnd), (void*)24);
    ck_assert_ptr_eq(keyEnd, str + strlen(str) - 1);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_query_triehasOneInvalidWord_isNULL){
    Trie * test = Trie_new();
    const char * keyEnd = (char *)23;
    const char * str = "WODSD";
    Trie_insert(test, str, (void *)24);
    ck_assert_ptr_eq(Trie_query(test, "WODS", &keyEnd), NULL);
    ck_assert_ptr_eq(Trie_query(test, "ASD", &keyEnd), NULL);
    ck_assert_ptr_eq(keyEnd, NULL);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_query_triehasMultipleInvalidWords_isNULL){
    Trie * test = Trie_new();
    const char * keyEnd = (char *)23;
    Trie_insert(test, "WADWA", (void *)24);
    Trie_insert(test, "ASDDszcx", (void *)24);
    Trie_insert(test, "AS@#@#DDszcx", (void *)24);
    Trie_insert(test, "A@#@$SDDszcx", (void *)24);
    Trie_insert(test, "ASDDszcx", (void *)24);
    Trie_insert(test, "4343ASDDszcx", (void *)24);
    Trie_insert(test, "ASDD21231szcx", (void *)24);
    Trie_insert(test, "ASDawdx", (void *)24);
    ck_assert_ptr_eq(Trie_query(test, "ASKVAL", &keyEnd), NULL);
    ck_assert_ptr_eq(keyEnd, NULL);
    Trie_free(test, (Destructor)destr);
} END_TEST

START_TEST (Trie_query_triehasMultipleWordsWithCommonLetters_lastValidPtr){
    Trie * test = Trie_new();
    const char * keyEnd = (char *)23;
    const char * str = "ABCDEFGHIJKLMNO";
    Trie_insert(test, "ABCD", (void *)24);
    Trie_insert(test, "ABCDEFG", (void *)25);
    Trie_insert(test, "ABCDEFGHJ", (void *)26);
    Trie_insert(test, "ABCDEFDGDS", (void *)27);
    Trie_insert(test, str, (void *)28);
    Trie_insert(test, "ABCDERERADSZXC", (void *)29);
    Trie_insert(test, "ABCDEFGHIJKLMNOSER", (void *)30);
    Trie_insert(test, "ABCDEFGHNdfsknzlz;c", (void *)31);
    ck_assert_ptr_eq(Trie_query(test, str, &keyEnd), (void*)28);
    ck_assert_ptr_eq(keyEnd, str + strlen(str) - 1);

    ck_assert_ptr_eq(Trie_query(test, "ABCD", &keyEnd), (void*)24);
    ck_assert_int_eq(*keyEnd, 'D');
    Trie_free(test, (Destructor)destr);
} END_TEST





Suite *test_suite(void);

int main(void) {
    setlocale(LC_CTYPE, "");
    Suite *s = test_suite();
    SRunner *sr = srunner_create(s);
    srunner_set_fork_status(sr, CK_NOFORK);  // important for debugging!

    srunner_run_all(sr, CK_NORMAL); // CK_VERBOSE

    int num_tests_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return num_tests_failed;
}

Suite *test_suite(void) {
    Suite *s = suite_create("Module");
    TCase *tc_sample;

    tc_sample = tcase_create("Data_structures");
    // LEXER
    tcase_add_test(tc_sample, Lexer_splitTokens_empty_empty);
    tcase_add_test(tc_sample, Lexer_splitTokens_oneNumber_oneNumberToken);
    tcase_add_test(tc_sample, Lexer_splitTokens_oneEqualityToken_oneEqualsToken);
    tcase_add_test(tc_sample, Lexer_splitTokens_invalidChar_errorCode);

    tcase_add_test(tc_sample, Lexer_splitTokens_twoEqualityToken_twoEqualityTokens);
    tcase_add_test(tc_sample, Lexer_splitTokens_unterminatedCharLiteral_errorCode);
    tcase_add_test(tc_sample, Lexer_splitTokens_unterminatedStringLiteral_errorCode);
    tcase_add_test(tc_sample, Lexer_splitTokens_invalidFloatingPointLiteral_errorCode);
    tcase_add_test(tc_sample, Lexer_splitTokens_oneKeyword_oneKeywordToken);
    tcase_add_test(tc_sample, Lexer_splitTokens_oneIdentifier_oneIdentifierToken);
    tcase_add_test(tc_sample, Lexer_splitTokens_validEspaceSequenceCharacterLiteral_oneCharToken);
    tcase_add_test(tc_sample, Lexer_splitTokens_validEspaceSequenceInStringLiteral_onePowToken);
    tcase_add_test(tc_sample, Lexer_splitTokens_invalidEspaceSequenceCharacterLiteral_errorCode);
    tcase_add_test(tc_sample, Lexer_splitTokens_invalidEspaceSequenceStringLiteral_errorCode);

    tcase_add_test(tc_sample, Lexer_splitTokens_emptyStringLiteral_oneLiteralToken);


    // LIST
    tcase_add_test(tc_sample, List_isEmpty_newList_isTrue);
    tcase_add_test(tc_sample, List_isEmpty_oneElement_isFalse);
    tcase_add_test(tc_sample, List_isEmpty_onePushedOnePopped_isTrue);
    // TRIE
    tcase_add_test(tc_sample, Trie_insert_emtyTrie_isTrue);
    tcase_add_test(tc_sample, Trie_insert_hasOneUniqueWord_isTrue);
    tcase_add_test(tc_sample, Trie_insert_hasOneWordWithCommonLetter_isTrue);
    tcase_add_test(tc_sample, Trie_insert_hasTwoWordsWithCommonLetter_isTrue);
    tcase_add_test(tc_sample, Trie_insert_hasSameWord_isFalse);
    tcase_add_test(tc_sample, Trie_insert_zeroLengthString_isTrue);
    tcase_add_test(tc_sample, Trie_insert_zeroLengthStringTwice_isFalse);
    tcase_add_test(tc_sample, Trie_insert_hasMultipleWordsWithCommonLeter_isTrue);
    tcase_add_test(tc_sample, Trie_insert_isPartOfAPresentWord_isTrue);

    tcase_add_test(tc_sample, Trie_search_hasOneWord_isTrue);
    tcase_add_test(tc_sample, Trie_search_hasTwoUniqueWords_isTrue);
    tcase_add_test(tc_sample, Trie_search_hasTwoWordsWithCommonLetter_isTrue);
    tcase_add_test(tc_sample, Trie_search_hasMultipleWordsWithCommonLetter_isTrue);
    tcase_add_test(tc_sample, Trie_search_hasNoWord_isFalse);
    tcase_add_test(tc_sample, Trie_search_hasOnlyAlikeWord_isFalse);
    tcase_add_test(tc_sample, Trie_search_trieIsEmpty_isFalse);
    
    tcase_add_test(tc_sample, Trie_remove_hasOneWord_validPtr);
    tcase_add_test(tc_sample, Trie_remove_hasTwoWordsWithCommonLetter_validPtr);
    tcase_add_test(tc_sample, Trie_remove_hasMultipleWordsWithCommonLetter_validPtr);
    tcase_add_test(tc_sample, Trie_remove_emptyString_validPtr);
    tcase_add_test(tc_sample, Trie_remove_isPartOfAPresentWord_validPtr);
    tcase_add_test(tc_sample, Trie_remove_emptyTrie_nullFalse);
    tcase_add_test(tc_sample, Trie_remove_hasNoSuchWord_nullFalse);
    tcase_add_test(tc_sample, Trie_remove_hasNoSuchWordWhichIsAPartOfAnother_nullFalse);


    tcase_add_test(tc_sample, Trie_isEmpty_newTrie_isTrue);
    tcase_add_test(tc_sample, Trie_isEmpty_trieWithOneWord_isFalse);
    tcase_add_test(tc_sample, Trie_isEmpty_trieWithEmptyString_isFalse);
    tcase_add_test(tc_sample, Trie_isEmpty_trieWithTwoWords_isFalse);
    tcase_add_test(tc_sample, Trie_isEmpty_trieInsertedAndRemovedWord_isTrue);

    tcase_add_test(tc_sample, Trie_query_trieIsEmpty_isNULL);
    tcase_add_test(tc_sample, Trie_query_trieHasOneValidWord_validPtr);
    tcase_add_test(tc_sample, Trie_query_trieHasTwoWordsWithCommonLetter_validPtr);
    tcase_add_test(tc_sample, Trie_query_triehasOneInvalidWord_isNULL);
    tcase_add_test(tc_sample, Trie_query_triehasMultipleInvalidWords_isNULL);
    tcase_add_test(tc_sample, Trie_query_triehasMultipleWordsWithCommonLetters_lastValidPtr);



    suite_add_tcase(s, tc_sample);

    return s;
}