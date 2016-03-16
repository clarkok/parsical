#include <iostream>

#include "parsical-parser.hpp"
#include "print-visitor.hpp"

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
    parsical::PrintVisitor pv(std::cout);

    auto *ast = pp.parse_file(*argv);

    ast->accept(&pv);

    return 0;
}
