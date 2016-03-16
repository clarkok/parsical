#ifndef _PARSICAL_PRINT_VISITOR_HPP_
#define _PARSICAL_PRINT_VISITOR_HPP_

#include <iostream>
#include "parsical-parser.hpp"

namespace parsical
{

class PrintVisitor : public parser::Visitor
{
    std::ostream &_os;
    int _indent;

public:
    PrintVisitor(std::ostream &os)
        : _os(os), _indent(0)
    { }

    virtual ~PrintVisitor() = default;

    define_visitor
};

}

#endif
