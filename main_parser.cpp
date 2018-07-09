#include "lexertl/stream_shared_iterator.hpp"
#include <fstream>
#include "lexertl/generator.hpp"
#include <iostream>
#include <cui.h>
#include "lexertl/lookup.hpp"
#include "lexer.h"
#include "var_substring.h"
#include "var_list.h"

template <typename TString>
void printError(const TString & errorString){
    Calamity::conout << Cui::fgnd(5, 1, 0) << Cui::bold
                     << ca("SIGSEGV: ")
                     << errorString
                     << Cui::reset << Calamity::endline;
}

int main () {
    using Calamity::conout;
    using Calamity::endline;
    using Calamity::String;
    using Calamity::Lexer;
    using Calamity::Exception;
    using namespace Cui;
    std::locale::global(std::locale(""));
    String code;
    try {
        code = String::readFromFile("../main.js");
    } catch (const Exception & exception){
        printError(exception);
        return EXIT_FAILURE;
    }
    Lexer lexer;
    String errorStr(lexer.splitTokens(&code));
    if (!errorStr.empty()){
        printError(errorStr);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}