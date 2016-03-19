#ifndef _PARSICAL_NORMALIZE_HPP_
#define _PARSICAL_NORMALIZE_HPP_

#include <stack>

#include "parsical-parser.hpp"

namespace parsical {

class NormalizeVisitor : public parser::Visitor
{
    parser::TreeNode *_current;
    int _counter = 0;
    parser::Grammar *_grammar;
public:
    NormalizeVisitor(parser::Grammar *node)
        : _grammar(node)
    { node->accept(this); }

    virtual ~NormalizeVisitor() = default;

    define_visitor_visit(parser::SentenceRule)
    define_visitor_visit(parser::TokenRule_Rule2)
    define_visitor_visit(parser::TokenRule_Rule4)
    define_visitor_visit(parser::SentencePart)
};

}

#endif