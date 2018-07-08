#include <lexertl/generator.hpp>
#include <lexertl/generate_cpp.hpp>
#include <lexertl/debug.hpp>
#include <fstream>
#include <cstdlib>
#include "cui.h"
#include "token.h"
#include "std_ext.h"

int main (int argc, char ** argv) {
    using namespace Calamity;
    using namespace Cui;
    namespace ltl = lexertl;
    const auto err(fgnd(5, 1, 0));

    if (argc != 2){
        conout << err << bold
               << ca("No input file is specified")
               << reset << newline;
        return EXIT_FAILURE;
    }

    const char * GEN_PATH(argv[1]);
    conout << bold
           << ca("Generating lexer source code into \"")
           << GEN_PATH
           << ca('\"')
           << bold_off << newline;

    std::ofstream genFile(GEN_PATH);
    if (!genFile.is_open()){
        conout << err << bold
               << ca("SIGSEGV: failed to open file \"")
               << GEN_PATH
               << ca("\" for code generation")
               << reset << newline;
        return EXIT_FAILURE;
    }

    using ltl_rules = ltl::basic_rules<cachar_t, cachar_t>;
    using ltl_state_machine = ltl::basic_state_machine<cachar_t>;
    using ltl_generator = ltl::basic_generator<ltl_rules, ltl_state_machine>;
    using ltl_debug = ltl::basic_debug<ltl_state_machine, cachar_t>;

    ltl_rules rules;
    ltl_state_machine sm;
    try {
        rules = Token::makeLexertlRules();
        ltl_generator::build(rules, sm);
    } catch (const lexertl::runtime_error & error){
        conout << err << bold
               << ca("SIGSEGV: failed to build lexer (")
               << error.what()
               << ca(')')
               << reset << newline;
        return EXIT_FAILURE;
    }

    sm.minimise();

    ltl_debug::dump(sm, conout);

    genFile << "#include <lexertl/match_results.hpp>\n\n"
               "namespace Calamity {\n\t"
               "namespace ltlgen {\n\n";
    lexertl::table_based_cpp::generate_cpp("lookup", sm, false, genFile);
    genFile << "\n\n\t} // namespace ltlgen\n"
               "} // namespace Calamity\n\n\n";

    genFile.close();

    conout << reverse
           << ca("<--- Code generation completed --->")
           << reverse_off
           << newline;
    return EXIT_SUCCESS;
}