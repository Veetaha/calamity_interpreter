#include "lexertl/stream_shared_iterator.hpp"
#include <fstream>
#include "lexertl/generator.hpp"
#include <iostream>
#include <cui.h>
#include "lexertl/lookup.hpp"
#include "lexer.h"
#include "var_substring.h"

int main () {
    using namespace Calamity;
    std::locale::global(std::locale(""));

    String code;
    try {
        code = String::readFromFile("../main.js");
    } catch (const MessageException & exception){
        std::wcout << "SIGSEGV: " << exception.what();
        return EXIT_FAILURE;
    }
    Lexer lexer;
    String errorStr(lexer.splitTokens(&code));
    std::wcout << Cui::reverse
               << Cui::bold
               << L"Lexical analysis finished with code:"
               << Cui::reset
               << L'\n'
               << errorStr;

    return EXIT_SUCCESS;
}