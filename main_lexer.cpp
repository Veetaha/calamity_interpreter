#include "lexertl/stream_shared_iterator.hpp"
#include <fstream>
#include "lexertl/generator.hpp"
#include <iostream>
#include "lexertl/lookup.hpp"
#include "lexer.h"
#include "defs.h"
#include "var_substring.h"
#include "cui.h"

#include "var_list.h"
int main () {
    using namespace Cala;
    // default locale is before main function in defs.h header
    try {
        conout << Lexer::splitTokensFromFile("../main.js");
    } catch (const Exception & exception){
        conout << ca("SIGSEGV: ") << exception.what();
        return EXIT_FAILURE;
    }



    return EXIT_SUCCESS;
}