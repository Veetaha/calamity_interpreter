#include <lexertl/generator.hpp>
#include <lexertl/generate_cpp.hpp>
#include <lexertl/debug.hpp>
#include <fstream>
#include <cstdlib>
#include "cui.h"
#include "token.h"
#include "std_ext.h"


int main () {
    using namespace Calamity;
    namespace ltl = lexertl;

    constexpr const char * GEN_PATH("../include/lexertl_gen.h");
    std::ofstream genFile(GEN_PATH);
    if (!genFile.is_open()){
        std::cout << "SIGSEGV: failed to open file \""<< GEN_PATH << "\" for code generation";
        return EXIT_FAILURE;
    }

    ltl::u16rules rules(Token::makeLexertlRules());
    ltl::u16state_machine sm;

    lexertl::u16generator::build(rules, sm);
    sm.minimise();

    genFile << "#include <lexertl/match_results.hpp>\n\n"
               "namespace Calamity {\n\t"
               "namespace ltlgen {\n\n";
    lexertl::table_based_cpp::generate_cpp("lookup", sm, false, genFile);
    genFile << "\n\n\t} // namespace ltlgen\n"
               "} // namespace Calamity\n\n\n";
    genFile.close();

//    ltl::u16debug::dump(sm, std::cout);
    std::cout << Cui::reverse <<"<--- Code generation completed --->" << Cui::reverse_off << '\n';
    return EXIT_SUCCESS;
}