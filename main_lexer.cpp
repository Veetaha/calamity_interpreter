#include "lexertl/stream_shared_iterator.hpp"
#include <fstream>
#include "lexertl/generator.hpp"
#include <iostream>
#include <cui.h>
#include "lexertl/lookup.hpp"
#include "lexer.h"
#include "var_substring.h"

#include "var_list.h"
int main () {
    using Calamity::conout;
    using Calamity::endline;
    using Calamity::String;
    using Calamity::Lexer;
    using Calamity::Exception;
    std::locale::global(std::locale(""));
    String code;
    try {
        code = String::readFromFile("../main.js");
    } catch (const Exception & exception){
        conout << ca("SIGSEGV: ") << exception.what();
        return EXIT_FAILURE;
    }
    Lexer lexer;
    String errorStr(lexer.splitTokens(&code));
    conout << Cui::reverse
               << Cui::bold
               << ca("Lexical analysis finished with code:")
               << Cui::reset
               << endline
               << errorStr
               << endline
               << ca("Parsed tokens:\n")
               << lexer;
    return EXIT_SUCCESS;
}