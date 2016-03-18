#ifndef _PARSICAL_SYMBOL_INFO_HPP_
#define _PARSICAL_SYMBOL_INFO_HPP_

#include <string>
#include <map>

#include "parsical-parser.hpp"

namespace parsical {

class SymbolInfoVisitor : public parser::Visitor
{
    std::map<std::string, parser::TreeNode*> _symbol_table;
    std::map<std::string, parser::TreeNode*> _fragment_table;

public:
    SymbolInfoVisitor() = default;
    virtual ~SymbolInfoVisitor() = default;

    static void reportDuplicated(parser::TId *met, parser::TreeNode *prev);
    static void reportMissing(parser::TId *met);

    define_visitor_visit(parser::TokenRule_Rule1)
    define_visitor_visit(parser::TokenRule_Rule2)
    define_visitor_visit(parser::TokenRule_Rule3)
    define_visitor_visit(parser::SentenceRule_Rule1)
    define_visitor_visit(parser::SentenceRule_Rule2)

    auto symbolBegin() -> decltype(SymbolInfoVisitor::_symbol_table.begin())
    { return _symbol_table.begin(); }

    auto symbolEnd() -> decltype(SymbolInfoVisitor::_symbol_table.end())
    { return _symbol_table.end(); }

    auto fragmentBegin() -> decltype(SymbolInfoVisitor::_fragment_table.begin())
    { return _fragment_table.begin(); }

    auto fragmentEnd() -> decltype(SymbolInfoVisitor::_fragment_table.end())
    { return _fragment_table.end(); }
};

}

#endif
