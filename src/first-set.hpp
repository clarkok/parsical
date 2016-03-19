#ifndef _PARSICAL_FIRST_SET_HPP_
#define _PARSICAL_FIRST_SET_HPP_

#include <set>
#include <map>

#include "parsical-parser.hpp"
#include "symbol-info.hpp"

namespace parsical {

typedef std::set<int> CharSet;
static const int EMPTY_CHAR = -1;

class FirstSetVisitor : public parser::Visitor
{
    SymbolInfoVisitor *_symbols;
    std::map<std::string, CharSet> _first_set;

    CharSet calculateSentenceUnary(parser::SentenceUnary *sentence_unary);
    CharSet calculateSentencePart(parser::SentencePart *sentence_part);
    CharSet calculateSentenceBranch(parser::SentenceBranch *sentence_branch);
    void calculateSentence(std::string name, parser::SentenceDecl *sentence_decl);

public:
    FirstSetVisitor(SymbolInfoVisitor *symbols)
        : _symbols(symbols)
    { }

    virtual ~FirstSetVisitor() = default;

    inline CharSet &
    getFirstSet(std::string name)
    { return _first_set[name]; }

    define_visitor_visit(parser::SentenceRule)
};

}

#endif
