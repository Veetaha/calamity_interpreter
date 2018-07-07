#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include <check.h>
#include <list.h>
#include <lexer.h>
#include <sstring.h>
#include <ast.h>
#include <parser.h>
#include <tree_ext.h>
#include <location.h>

void destr(void * ptr) {return;}


START_TEST (Parser_empty_empty) {
    List * list = List_new();
    String * code = String_toNewString("");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast != NULL);
    ck_assert_int_eq(Tree_size(ast), 1);
    ck_assert(AstNode_typeAt(ast->val) == AstNodeType_CALAMITY);

    Tree_freeWhole(ast, (Destructor)AstNode_freeWhole);
    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_delcareVar_declAndId) {
    List * list = List_new();
    String * code = String_toNewString("new variable;");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast != NULL);
    ck_assert_int_eq(Tree_size(ast), 3);
    ck_assert(AstNode_typeAt((Tree_childAt(ast, 0)->val)) == AstNodeType_DECLARATION);
    ck_assert(AstNode_typeAt(Tree_at(Tree_childAt(Tree_childAt(ast, 0), 0))) == AstNodeType_IDENTIFIER);

    Tree_freeWhole(ast, (Destructor)AstNode_freeWhole);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_initVar_declAndIdandExpr) {
    List * list = List_new();
    String * code = String_toNewString("new identifier = 2 + 2 * 2;");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);  
    ck_assert(ast != NULL);
    Tree * decl = Tree_childAt(ast, 0);
    ck_assert(AstNode_typeAt(Tree_at(Tree_childAt(decl, 1))) == AstNodeType_PLUS);

    Tree_freeWhole(ast, (Destructor)AstNode_freeWhole);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_initWithNoExpr) {
    List * list = List_new();
    String * code = String_toNewString("new identifier = ;");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast == NULL);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_assignment_IDAndExpr) {
    List * list = List_new();
    String * code = String_toNewString("identifier = 78 ++ [23, 2 * 2];");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast != NULL);
    ck_assert_int_eq(Tree_size(ast), 10);
    Tree * assign = Tree_childAt(ast, 0);
    ck_assert(AstNode_typeAt(assign->val) == AstNodeType_ASSIGN);
    ck_assert(AstNode_typeAt(Tree_childAt(assign, 1)->val) == AstNodeType_CONCATENATE);

    Tree_freeWhole(ast, (Destructor)AstNode_freeWhole);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_getListElement_IDAndArglistOneArg) {
    List * list = List_new();
    String * code = String_toNewString("arr[2];");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast != NULL);
    
    ck_assert_int_eq(Tree_size(ast), 4);

    Tree_freeWhole(ast, (Destructor)AstNode_freeWhole);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_getSubList_IDAndArglistTwoArg) {
    List * list = List_new();
    String * code = String_toNewString("arr[i ... j];");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast != NULL);


    ck_assert_int_eq(Tree_size(ast), 5);

    Tree_freeWhole(ast, (Destructor)AstNode_freeWhole);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_emtyStatement_oneEmtyStatement) {
    List * list = List_new();
    String * code = String_toNewString(";");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast != NULL);
    ck_assert_int_eq(Tree_size(ast), 2);
    ck_assert(AstNode_typeAt(Tree_at(Tree_childAt(ast, 0))) == AstNodeType_BLANK_STATEMENT);

    Tree_freeWhole(ast, (Destructor)AstNode_freeWhole);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_emptyBlock_oneEmptyBlockStatement) {
    List * list = List_new();
    String * code = String_toNewString("{}");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast != NULL);

    ck_assert_int_eq(Tree_size(ast), 2);
    ck_assert(AstNode_typeAt(Tree_at(Tree_childAt(ast, 0))) == AstNodeType_BLOCK);


    Tree_freeWhole(ast, (Destructor)AstNode_freeWhole);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_assignCompundLiteral_exprWithCompoundLiteral) {
    List * list = List_new();
    String * code = String_toNewString("arr = [[23, 45], [23, 43]];");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast != NULL);
    ck_assert_int_eq(Tree_size(ast), 10);

    Tree_freeWhole(ast, (Destructor)AstNode_freeWhole);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_assigndToBoolean_exprWithBoolean) {
    List * list = List_new();
    String * code = String_toNewString("23 = true;");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast != NULL);

    ck_assert_int_eq(Tree_size(ast), 4);
    Tree * assign = Tree_childAt(ast, 0);
    ck_assert(AstNode_typeAt(Tree_childAt(assign, 1)->val) == AstNodeType_LITERAL);

    Tree_freeWhole(ast, (Destructor)AstNode_freeWhole);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_whileWithBolck) {
    List * list = List_new();
    String * code = String_toNewString("while(true){2 + 2;}");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast != NULL);
    ck_assert_int_eq(Tree_size(ast), 7);

    Tree_freeWhole(ast, (Destructor)AstNode_freeWhole);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_whileWithNoStatement) {
    List * list = List_new();
    String * code = String_toNewString("while(false)");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast == NULL);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_whileWithNoCondition) {
    List * list = List_new();
    String * code = String_toNewString("while 2 + 2");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast == NULL);


    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_caseWithBlock) {
    List * list = List_new();
    String * code = String_toNewString("if (23) {2+2; 8*8;}");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast != NULL);
    ck_assert_int_eq(Tree_size(ast), 10);

    Tree_freeWhole(ast, (Destructor)AstNode_freeWhole);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_caseWithBlockAndOtherwiseWithBlock) {
    List * list = List_new();
    String * code = String_toNewString("if (22) 2 + 2; else 5;");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast != NULL);

    ck_assert_int_eq(Tree_size(ast), 7);


    Tree_freeWhole(ast, (Destructor)AstNode_freeWhole);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_caseWithNoCondition) {
    List * list = List_new();
    String * code = String_toNewString("case");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast == NULL);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_caseWithNoStatement) {
    List * list = List_new();
    String * code = String_toNewString("case (2 ++ 2)");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast == NULL);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_caseAndOtherwiseWithNoStatement) {
    List * list = List_new();
    String * code = String_toNewString("case (true) otherwise");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast == NULL);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_singleOthewiseWithStatement) {
    List * list = List_new();
    String * code = String_toNewString("otherwise 2 + 2;");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast == NULL);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_funcWithNoArgs) {
    List * list = List_new();
    String * code = String_toNewString("stringify();");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast != NULL);

    ck_assert_int_eq(Tree_size(ast), 3);

    Tree_freeWhole(ast, (Destructor)AstNode_freeWhole);
    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_funcWithArgs) {
    List * list = List_new();
    String * code = String_toNewString("digify(23 , 54, [32]);");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast != NULL);

    ck_assert_int_eq(Tree_size(ast), 7);


    Tree_freeWhole(ast, (Destructor)AstNode_freeWhole);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_funcWithoutParentheses) {
    List * list = List_new();
    String * code = String_toNewString("print 76;");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast == NULL);

    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST

START_TEST (Parser_unaryOperatorBetweenBinary) {
    List * list = List_new();
    String * code = String_toNewString("12 ^ -3;");
    Lexer_splitTokens(String_readOnly(code), list);
    Tree * ast = Parser_buildNewAstTree(list, code);
    ck_assert(ast == NULL);


    Lexer_clearTokens(list);
    List_free(list);
    String_free(code);
} END_TEST







Suite *test_suite(void);

int main(void) {
    setlocale(LC_CTYPE, "");
    Suite *s = test_suite();
    SRunner *sr = srunner_create(s);
    // srunner_set_fork_status(sr, CK_NOFORK);  // important for debugging!

    srunner_run_all(sr, CK_NORMAL); // CK_VERBOSE

    int num_tests_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return num_tests_failed;
}

Suite *test_suite(void) {
    Suite *s = suite_create("Module");
    TCase *tc_sample;

    tc_sample = tcase_create("Data_structures");
    // PARSER
    tcase_add_test(tc_sample, Parser_empty_empty);
    tcase_add_test(tc_sample, Parser_delcareVar_declAndId);
    tcase_add_test(tc_sample, Parser_initVar_declAndIdandExpr);
    tcase_add_test(tc_sample, Parser_initWithNoExpr);
    tcase_add_test(tc_sample, Parser_assignment_IDAndExpr);
    tcase_add_test(tc_sample, Parser_getListElement_IDAndArglistOneArg);
    tcase_add_test(tc_sample, Parser_getSubList_IDAndArglistTwoArg);
    tcase_add_test(tc_sample, Parser_emtyStatement_oneEmtyStatement);
    tcase_add_test(tc_sample, Parser_emptyBlock_oneEmptyBlockStatement);
    tcase_add_test(tc_sample, Parser_assignCompundLiteral_exprWithCompoundLiteral);
    tcase_add_test(tc_sample, Parser_assigndToBoolean_exprWithBoolean);
    tcase_add_test(tc_sample, Parser_whileWithBolck);
    tcase_add_test(tc_sample, Parser_whileWithNoStatement);
    tcase_add_test(tc_sample, Parser_whileWithNoCondition);
    tcase_add_test(tc_sample, Parser_caseWithBlock);
    tcase_add_test(tc_sample, Parser_caseWithBlockAndOtherwiseWithBlock);
    tcase_add_test(tc_sample, Parser_caseWithNoCondition);
    tcase_add_test(tc_sample, Parser_caseWithNoStatement);
    tcase_add_test(tc_sample, Parser_caseAndOtherwiseWithNoStatement);
    tcase_add_test(tc_sample, Parser_singleOthewiseWithStatement);
    tcase_add_test(tc_sample, Parser_funcWithNoArgs);
    tcase_add_test(tc_sample, Parser_funcWithArgs);
    tcase_add_test(tc_sample, Parser_funcWithoutParentheses);
    tcase_add_test(tc_sample, Parser_unaryOperatorBetweenBinary);


    suite_add_tcase(s, tc_sample);

    return s;
}