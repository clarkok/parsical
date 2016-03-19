#include <iostream>
#include <fstream>

#include "driver.hpp"

using namespace std;

int
main(int argc, char **argv)
{
    --argc; ++argv;
    if (!argc) {
        std::cout << "No input file" << std::endl;
        return -1;
    }

    parsical::Driver driver(argc, argv);

    driver.generateGlobalLexer();
    driver.generateStructureLexers();

    return 0;
}
