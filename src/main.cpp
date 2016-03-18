#include <iostream>
#include <fstream>

#include "parsical-parser.hpp"
#include "print-visitor.hpp"
#include "token-info.hpp"
#include "lexer.hpp"
#include "symbol-info.hpp"

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

    parsical::SymbolInfoVisitor si;
    ast->accept(&si);

    std::cout << "symbols:" << std::endl;
    for (auto iter = si.symbolBegin(); iter != si.symbolEnd(); ++iter) {
        std::cout << "\t" << iter->first << std::endl;
    }

    std::cout << "fragment:" << std::endl;
    for (auto iter = si.fragmentBegin(); iter != si.fragmentEnd(); ++iter) {
        std::cout << "\t" << iter->first << std::endl;
    }

    /*
    parsical::Lexer lexer_generator(ast);

    lexer_generator.outputFATables(std::cout);

    std::ofstream ftoken("./output.token");
    lexer_generator.outputTokenEnum(ftoken);

    std::ofstream fnfa("./output.nfa");
    lexer_generator.outputNFA(fnfa);

    std::ofstream fdfa("./output.dfa");
    lexer_generator.outputDFA(fdfa);
    */

    return 0;
}
