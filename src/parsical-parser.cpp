#include <iostream>
#include <cstdio>

#include "parsical-parser.hpp"

using namespace parsical;
using namespace parsical::parser;

extern FILE *yyin;
Grammar *result;

void
ParsicalParser::parse_file(const std::string &filename)
{
    yyin = std::fopen(filename.c_str(), "r");
}
