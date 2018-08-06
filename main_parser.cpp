#include <fstream>
#include <iostream>
#include "defs.h"
#include "lexertl/lookup.hpp"
#include "lexer.h"
#include "var_substring.h"
#include "parser.h"
#include "var_list.h"
#include "ast.h"

int main () {
    using namespace Cala;
    try {
        GrammaticalInfo grammaticalInfo(Parser::parse(Lexer::splitTokensFromFile("../main.js")));
        String astString;
        conout << grammaticalInfo.ast().appendToString(astString);
    } catch (const Exception & exception){
        conout << exception.what();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}