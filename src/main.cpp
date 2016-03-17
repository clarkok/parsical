#include <iostream>
#include <fstream>

#include "parsical-parser.hpp"
#include "print-visitor.hpp"
#include "token-info.hpp"
#include "lexer.hpp"

using namespace std;

int
main(int argc, char **argv)
{
    --argc; ++argv;
    if (!argc) {
        std::cout << "No input file" << std::endl;
        return -1;
    }

    parsical::parser::ParsicalParser pp;
    auto *ast = pp.parse_file(*argv);

    parsical::Lexer lexer_generator(ast);

    lexer_generator.outputFATables(std::cout);

    std::ofstream ftoken("./output.token");
    lexer_generator.outputTokenEnum(ftoken);

    std::ofstream fnfa("./output.nfa");
    lexer_generator.outputNFA(fnfa);

    std::ofstream fdfa("./output.dfa");
    lexer_generator.outputDFA(fdfa);

    return 0;
}
