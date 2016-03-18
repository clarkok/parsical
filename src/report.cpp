#include <iostream>
#include <cstdlib>

#include "report.hpp"
#include "parsical-parser.hpp"

using namespace parsical;

void
Report::error(std::string msg) const
{
    std::cout << "ERROR: " << msg << std::endl;
    exit(-1);
}

void
Report::warn(std::string msg) const
{
    std::cout << "WARN:  " << msg << std::endl;
}

std::string
Report::positionedMessage(parser::Location &loc, std::string msg, std::string info)
{
    return locationToString(loc) + "\t" + msg + "\n\t" + info + "\n";
}

std::string
Report::locationToString(parser::Location &loc)
{ return loc.file + " " + std::to_string(loc.line) + ":" + std::to_string(loc.column); }

Report &
Report::getReport()
{
    static Report _global_report;
    return _global_report;
}
