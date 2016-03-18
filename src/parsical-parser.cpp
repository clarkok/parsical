#include <iostream>
#include <cstdio>
#include <cassert>

#include "parsical-parser.hpp"

using namespace parsical;
using namespace parsical::parser;

extern FILE *yyin;
extern int yydebug;

int yyparse();

namespace parsical {
namespace parser {
Grammar *result;
}
}

void
Visitor::visit(TreeNode *)
{ assert(false); }

void
Visitor::visit(TokenNode *)
{ }

void
Visitor::visit(Grammar *node)
{
    for (auto &rep : node->grammar_rep_1) {
        rep.rule->accept(this);
    }
}

void
Visitor::visit(Rule_Rule1 *node)
{
    node->token_rule->accept(this);
}

void
Visitor::visit(Rule_Rule2 *node)
{
    node->sentence_rule->accept(this);
}

void
Visitor::visit(Rule *node)
{
    switch (node->getRule()) {
        case 1: dynamic_cast<Rule_Rule1*>(node)->accept(this);  break;
        case 2: dynamic_cast<Rule_Rule2*>(node)->accept(this);  break;
        default: assert(false);
    }
}

void
Visitor::visit(TokenRule_Rule1 *node)
{
    node->id->accept(this);
    node->regex->accept(this);
}

void
Visitor::visit(TokenRule_Rule2 *node)
{
    node->id->accept(this);
    node->sentence_decl->accept(this);
}

void
Visitor::visit(TokenRule_Rule3 *node)
{
    node->id->accept(this);
    node->regex->accept(this);
}

void
Visitor::visit(TokenRule *node)
{
    switch (node->getRule()) {
        case 1: dynamic_cast<TokenRule_Rule1*>(node)->accept(this); break;
        case 2: dynamic_cast<TokenRule_Rule2*>(node)->accept(this); break;
        default: assert(false);
    }
}

void
Visitor::visit(SentenceRule *node)
{
    switch (node->getRule()) {
        case 1: dynamic_cast<SentenceRule_Rule1*>(node)->accept(this); break;
        case 2: dynamic_cast<SentenceRule_Rule2*>(node)->accept(this); break;
        default: assert(false);
    }
}

void
Visitor::visit(SentenceRule_Rule1 *node)
{
    node->id->accept(this);
    node->sentence_decl->accept(this);
}

void
Visitor::visit(SentenceRule_Rule2 *node)
{
    node->id->accept(this);
    node->sentence_decl->accept(this);
}

void
Visitor::visit(SentenceDecl *node)
{
    node->sentence_branch->accept(this);
    for (auto &rep : node->sentence_decl_rep_1) {
        rep.sentence_branch->accept(this);
    }
}

void
Visitor::visit(SentenceBranch *node)
{
    for (auto &sentence_part : node->sentence_part_rep) {
        sentence_part->accept(this);
    }
}

void
Visitor::visit(SentencePart *node)
{
    node->sentence_unary->accept(this);
    if (node->sentence_decorate_opt) node->sentence_decorate_opt->accept(this);
    if (node->sentence_repeatition_opt) node->sentence_repeatition_opt->accept(this);
}

void
Visitor::visit(SentenceUnary_Rule1 *node)
{
    node->id->accept(this);
}

void
Visitor::visit(SentenceUnary_Rule2 *node)
{
    node->string->accept(this);
}

void
Visitor::visit(SentenceUnary_Rule3 *node)
{
    node->sentence_branch->accept(this);
}

void
Visitor::visit(SentenceUnary *node)
{
    switch (node->getRule()) {
        case 1: dynamic_cast<SentenceUnary_Rule1*>(node)->accept(this); break;
        case 2: dynamic_cast<SentenceUnary_Rule2*>(node)->accept(this); break;
        case 3: dynamic_cast<SentenceUnary_Rule3*>(node)->accept(this); break;
        default: assert(false);
    }
}

void
Visitor::visit(SentenceDecorate *node)
{
    node->token->accept(this);
}

void
Visitor::visit(SentenceRepeatition_Rule1 *node)
{
    node->token->accept(this);
}

void
Visitor::visit(SentenceRepeatition_Rule2 *node)
{
    node->token->accept(this);
}

void
Visitor::visit(SentenceRepeatition_Rule3 *node)
{
    node->token->accept(this);
}

void
Visitor::visit(SentenceRepeatition *node)
{
    switch (node->getRule()) {
        case 1: dynamic_cast<SentenceRepeatition_Rule1*>(node)->accept(this);   break;
        case 2: dynamic_cast<SentenceRepeatition_Rule2*>(node)->accept(this);   break;
        case 3: dynamic_cast<SentenceRepeatition_Rule3*>(node)->accept(this);   break;
        default: assert(false);
    }
}

void
Visitor::visit(TId *node)
{ (void)node; }

void
Visitor::visit(TString *node)
{
    for (auto &string_char : node->string_rep_1) {
        string_char->accept(this);
    }
}

void
Visitor::visit(TStringChar *node)
{
    if (node->string_non_trans_char)    node->string_non_trans_char->accept(this);
    if (node->string_trans_char)        node->string_trans_char->accept(this);
}

void
Visitor::visit(TStringNonTransChar *node)
{ (void)node; }

void
Visitor::visit(TStringTransChar *node)
{ (void)node; }

void
Visitor::visit(TRegex *node)
{
    node->regex_content->accept(this);
}

void
Visitor::visit(TRegexContent *node)
{
    node->regex_branch->accept(this);
    for (auto &rep : node->regex_content_rep_1) {
        rep.regex_branch->accept(this);
    }
}

void
Visitor::visit(TRegexBranch *node)
{
    for (auto &regex_part : node->regex_part_plus) {
        regex_part->accept(this);
    }
}

void
Visitor::visit(TRegexPart *node)
{
    node->regex_unary->accept(this);
    if (node->regex_repeatition_opt) node->regex_repeatition_opt->accept(this);
}

void
Visitor::visit(TRegexNeg *node)
{ (void)node; }

void
Visitor::visit(TRegexUnary_Rule1 *node)
{
    node->string_char->accept(this);
}

void
Visitor::visit(TRegexUnary_Rule2 *node)
{
    node->regex_content->accept(this);
}

void
Visitor::visit(TRegexUnary_Rule3 *node)
{
    if (node->regex_neg_opt) node->regex_neg_opt->accept(this);
    for (auto &regex_range : node->regex_range_plus) {
        regex_range->accept(this);
    }
}

void
Visitor::visit(TRegexUnary *node)
{
    switch (node->getRule()) {
        case 1: dynamic_cast<TRegexUnary_Rule1*>(node)->accept(this); break;
        case 2: dynamic_cast<TRegexUnary_Rule2*>(node)->accept(this); break;
        case 3: dynamic_cast<TRegexUnary_Rule3*>(node)->accept(this); break;
        default: assert(false);
    }
}

void
Visitor::visit(TRegexRange_Rule1 *node)
{
    node->string_char->accept(this);
}

void
Visitor::visit(TRegexRange_Rule2 *node)
{
    node->string_char_1->accept(this);
    node->string_char_2->accept(this);
}

void
Visitor::visit(TRegexRange *node)
{
    switch (node->getRule()) {
        case 1: dynamic_cast<TRegexRange_Rule1*>(node)->accept(this); break;
        case 2: dynamic_cast<TRegexRange_Rule2*>(node)->accept(this); break;
        default: assert(false);
    }
}

void
Visitor::visit(TRegexRepeatition *node)
{ (void)node; }

Grammar *
ParsicalParser::parse_file(const std::string &filename)
{
    yyin = std::fopen(filename.c_str(), "r");
    yydebug = 1;

    yyparse();

    return result;
}
