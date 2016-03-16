#include <iostream>

#include "parsical-parser.hpp"

using namespace parsical;
using namespace parsical::Parser;

ParsicalParser::ParsicalParser()
    : _location("", 0, 0)
{ }

void
ParsicalParser::parse_file(const std::string &filename)
{
}
