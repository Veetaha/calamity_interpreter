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
#include <interpreter.h>



START_TEST (Parser_empty_empty) {
    String * code = String_toNewString("");
    String * output = String_new();
    DynamicType * returned = Calamity_execute(code);

    
    String_free(code);
    String_free(output);
} END_TEST

START_TEST (Calamity_empty_NULL_Empty){
    String * code = String_toNewString("");
    String * output = String_new();
    String * expectedOut = String_toNewString("");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert_ptr_eq(status, NULL);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_singleVardecl_NULL_Emtpy){
    String * code = String_toNewString("new identifier;");
    String * output = String_new();
    String * expectedOut = String_toNewString("");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert_ptr_eq(NULL, status);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_singleFuncdef_NULL_Empty){
    String * code = String_toNewString("define function(someArgument1, someArgument2){}");
    String * output = String_new();
    String * expectedOut = String_toNewString("");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert_ptr_eq(status, NULL);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_rewrite(code, "define function(someArgument1, someArgument2);");
    status = Calamity_execute(code);

    ck_assert_ptr_eq(status, NULL);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_singleAssign_NULL_Empty){
    String * code = String_toNewString("new i; i = 12; print(i);");
    String * output = String_new();
    String * expectedOut = String_toNewString("12.000000");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert_ptr_eq(status, NULL);

    fflush(stdout);
    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_printString_NULL_String){
    String * code = String_toNewString("new string = \"The my program\"; print(string ++ \"concatenated\"); return [\"Array\"];");
    String * output = String_new();
    String * expectedOut = String_toNewString("The my programconcatenated");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert(status != NULL);
    DynamicType * expectedStatus = DynamicType_new(Type_VECTOR);
    DynamicType * string = DynamicType_new(Type_STRING);
    String_append(DynamicType_stringAt(string), "Array");
    List_addLast(DynamicType_listAt(expectedStatus), string);
    ck_assert(DynamicType_compare(status, expectedStatus) == 0);

    DynamicType_free(status);
    DynamicType_free(expectedStatus);
    
    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_returnPrintedString_undefined_String){
    String * code = String_toNewString("return print(\"Hey \\n hopa escapes\");");
    String * output = String_new();
    String * expectedOut = String_toNewString("Hey \n hopa escapes");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert(status != NULL);
    DynamicType * expectedStatus = DynamicType_new(Type_UNDEFINED);
    ck_assert(DynamicType_compare(status, expectedStatus) == 0);

    DynamicType_free(status);
    DynamicType_free(expectedStatus);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_1000RecursiveFunctionsPrint_NULL_Values){
    String * code = String_toNewString(
        "define rec(arg){   "
        "print(arg);        "
        " if (arg < 1000)   "
        "    rec(arg + 1);  "
        " else              "
        "    escape(\'\\b\');"
        "}                  "
        "rec(0);            "
    );
    String * output = String_new();
    String * expectedOut = String_toNewString("");
    for(float i = 0; i < 1001; ++i){
        String_appendFormat(expectedOut, "%.6f", i);
    }
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert(status != NULL);
    
    DynamicType * expectedStatus = DynamicType_new(Type_CHARACTER);
    DynamicType_setChar(expectedStatus, '\b');
    ck_assert(DynamicType_compare(status, expectedStatus) == 0);

    DynamicType_free(status);
    DynamicType_free(expectedStatus);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_printArrayInWhile_NULL_Array){
    String * code = String_toNewString(
        "new arr = [2, \"str\", \'c\', 5 != 2, 22 ++ 23];"
        "new i = 0;              "   
        "while (i < sizeof(arr)){"
        "    print(arr[i]);      "   
        "    i = i + 1;          "
        "}                       "
    );
    
    String * output = String_new();
    String * expectedOut = String_toNewString("2.000000strctrue22.000000 23.000000");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert_ptr_eq(status, NULL);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_escapeInMainCode_EscapedString){
    String * code = String_toNewString("escape(undefined);");
    String * output = String_new();
    String * expectedOut = String_toNewString("");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert(status != NULL);


    DynamicType * expectedStatus = DynamicType_new(Type_UNDEFINED);
    ck_assert(DynamicType_compare(status, expectedStatus) == 0);

    DynamicType_free(status);
    DynamicType_free(expectedStatus);


    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_useOfUndeclaredIdentifier_errorString_Empty){
    String * code = String_toNewString("new arr = 0; print(arr2);");
    String * output = String_new();
    String * expectedOut = String_toNewString("");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert(status != NULL);

    ck_assert(DynamicType_typeAt(status) == Type_STRING);
    DynamicType_free(status);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_useOfUndeclaredFunction_errorString_Empty){
    String * code = String_toNewString("define func(); func2();");
    String * output = String_new();
    String * expectedOut = String_toNewString("");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert(status != NULL);

    ck_assert(DynamicType_typeAt(status) == Type_STRING);
    DynamicType_free(status);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_arrayOutOfBounds_errorString_Empty){
    String * code = String_toNewString("new arr = [[434, 432], [34, [434, 535]]]; arr[1][2] = 0;");
    String * output = String_new();
    String * expectedOut = String_toNewString("");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert(status != NULL);

    ck_assert(DynamicType_typeAt(status) == Type_STRING);
    DynamicType_free(status);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_dimensionDesignatorException_errorString_Empty){
    String * code = String_toNewString("new arr = [[23, [23, [54, \"str\"]]]]; print(arr[0][0][1][1][1][1])");
    String * output = String_new();
    String * expectedOut = String_toNewString("");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert(status != NULL);

    ck_assert(DynamicType_typeAt(status) == Type_STRING);
    DynamicType_free(status);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_rangerIndexOutOfException_errorString_Empty){
    String * code = String_toNewString("new arr = array(20); print(arr[8...20]);");
    String * output = String_new();
    String * expectedOut = String_toNewString("");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert(status != NULL);

    ck_assert(DynamicType_typeAt(status) == Type_STRING);
    DynamicType_free(status);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_duplicatedFunctionArgumentsNamesAtFuncDef_errorString_Empty){
    String * code = String_toNewString("define function(arg, arg2, arg3, arg4, arg5, arg){}");
    String * output = String_new();
    String * expectedOut = String_toNewString("");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert(status != NULL);

    ck_assert(DynamicType_typeAt(status) == Type_STRING);
    DynamicType_free(status);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_duplicatedVariableNamesDeclaration_errorString_Empty){
    String * code = String_toNewString("new var; new var2; new vav2; new var;");
    String * output = String_new();
    String * expectedOut = String_toNewString("");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert(status != NULL);

    ck_assert(DynamicType_typeAt(status) == Type_STRING);
    DynamicType_free(status);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_duplicatedFunctionNamesDeclaration_errorString_Empty){
    String * code = String_toNewString("define func(){} define func2(arg); define func(arg, arg2)");
    String * output = String_new();
    String * expectedOut = String_toNewString("");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert(status != NULL);

    ck_assert(DynamicType_typeAt(status) == Type_STRING);
    DynamicType_free(status);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_useOfLocalVariableAfterItsBlock_errorString_Empty){
    String * code = String_toNewString("{new i = 0;} print(i);");
    String * output = String_new();
    String * expectedOut = String_toNewString("");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert(status != NULL);

    ck_assert(DynamicType_typeAt(status) == Type_STRING);
    DynamicType_free(status);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_returnAtMainCode_returnedValue_Empty){
    String * code = String_toNewString("return true;");
    String * output = String_new();
    String * expectedOut = String_toNewString("");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert(status != NULL);

    DynamicType * expectedStatus = DynamicType_new(Type_BOOLEAN);

    DynamicType_setBool(expectedStatus, true);

    ck_assert(DynamicType_compare(expectedStatus, status) == 0);
    DynamicType_free(status);
    DynamicType_free(expectedStatus);

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
} END_TEST
START_TEST (Calamity_invalidFunctionArgumentsCount_errorString_Empty){
    String * code = String_toNewString("define f(arg, arg){return true;}  new i = f(85);");
    String * output = String_new();
    String * expectedOut = String_toNewString("");
    freopen("test_output.gop", "w", stdout);
    DynamicType * status = Calamity_execute(code);
    ck_assert(status != NULL);

    ck_assert(DynamicType_typeAt(status) == Type_STRING);
    DynamicType_free(status);
    

    String_readFromPath("test_output.gop", output);
    ck_assert_int_eq(String_compare(output, expectedOut), 0);

    String_free(code);
    String_free(output);
    String_free(expectedOut);
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
    tcase_add_test(tc_sample, Calamity_empty_NULL_Empty);
    tcase_add_test(tc_sample, Calamity_singleVardecl_NULL_Emtpy);
    tcase_add_test(tc_sample, Calamity_singleFuncdef_NULL_Empty);
    tcase_add_test(tc_sample, Calamity_singleAssign_NULL_Empty);
    tcase_add_test(tc_sample, Calamity_printString_NULL_String);
    tcase_add_test(tc_sample, Calamity_returnPrintedString_undefined_String);
    tcase_add_test(tc_sample, Calamity_1000RecursiveFunctionsPrint_NULL_Values);
    tcase_add_test(tc_sample, Calamity_printArrayInWhile_NULL_Array);
    tcase_add_test(tc_sample, Calamity_escapeInMainCode_EscapedString);
    tcase_add_test(tc_sample, Calamity_useOfUndeclaredIdentifier_errorString_Empty);
    tcase_add_test(tc_sample, Calamity_useOfUndeclaredFunction_errorString_Empty);
    tcase_add_test(tc_sample, Calamity_arrayOutOfBounds_errorString_Empty);    
    tcase_add_test(tc_sample, Calamity_dimensionDesignatorException_errorString_Empty);    
    tcase_add_test(tc_sample, Calamity_rangerIndexOutOfException_errorString_Empty);    
    tcase_add_test(tc_sample, Calamity_duplicatedFunctionArgumentsNamesAtFuncDef_errorString_Empty);    
    tcase_add_test(tc_sample, Calamity_duplicatedVariableNamesDeclaration_errorString_Empty);    
    tcase_add_test(tc_sample, Calamity_duplicatedFunctionNamesDeclaration_errorString_Empty);    
    tcase_add_test(tc_sample, Calamity_useOfLocalVariableAfterItsBlock_errorString_Empty);    
    tcase_add_test(tc_sample, Calamity_returnAtMainCode_returnedValue_Empty);    
    tcase_add_test(tc_sample, Calamity_invalidFunctionArgumentsCount_errorString_Empty);    


    suite_add_tcase(s, tc_sample);

    return s;
}