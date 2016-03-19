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
    std::map<std::string, int> _symbol_id;
    std::map<int, std::string> _symbol_name;
    int _symbol_id_counter;

public:
    SymbolInfoVisitor(int symbol_id_counter = 0)
        : _symbol_id_counter(symbol_id_counter)
    { }

    virtual ~SymbolInfoVisitor() = default;

    static void reportDuplicated(parser::TId *met, parser::TreeNode *prev);
    static void reportMissing(parser::TId *met);

    define_visitor_visit(parser::TokenRule_Rule1)
    define_visitor_visit(parser::TokenRule_Rule2)
    define_visitor_visit(parser::TokenRule_Rule3)
    define_visitor_visit(parser::TokenRule_Rule4)
    define_visitor_visit(parser::SentenceRule)
    define_visitor_visit(parser::TString)

    inline bool
    hasSymbol(std::string name)
    { return _symbol_table.find(name) != _symbol_table.end(); }

    inline bool
    hasFragment(std::string name)
    { return _fragment_table.find(name) != _fragment_table.end(); }

    inline int
    getSymbolId(std::string name)
    { return _symbol_id[name]; }

    inline std::string
    getSymbolName(int id)
    { return _symbol_name[id]; }

    inline parser::TreeNode *
    findSymbol(std::string name)
    {
        auto iter = _symbol_table.find(name);
        if (iter == _symbol_table.end()) {
            return nullptr;
        }
        return iter->second;
    }

    inline parser::TreeNode *
    findFragment(std::string name)
    {
        auto iter = _fragment_table.find(name);
        if (iter == _fragment_table.end()) {
            return nullptr;
        }
        return iter->second;
    }

    inline auto
    symbolBegin() -> decltype(SymbolInfoVisitor::_symbol_table.begin())
    { return _symbol_table.begin(); }

    inline auto
    symbolEnd() -> decltype(SymbolInfoVisitor::_symbol_table.end())
    { return _symbol_table.end(); }

    inline auto
    fragmentBegin() -> decltype(SymbolInfoVisitor::_fragment_table.begin())
    { return _fragment_table.begin(); }

    inline auto
    fragmentEnd() -> decltype(SymbolInfoVisitor::_fragment_table.end())
    { return _fragment_table.end(); }
};

}

#endif
