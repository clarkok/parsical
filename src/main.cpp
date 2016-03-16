#include <iostream>

#include "parsical-parser.hpp"

using namespace std;

int
main(int argc, char **argv)
{
    --argc; ++argv;
    if (!argv) {
        std::cout << "No input file" << std::endl;
        return -1;
    }

    parsical::Parser::ParsicalParser pp;

    pp.parse_file(*argv);

    return 0;
}
