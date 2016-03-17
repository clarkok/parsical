#include <cassert>
#include <iostream>
#include <set>

#include "token-info.hpp"

using namespace parsical;

const int TokenInfoVisitor::TOKEN_START;

char
TokenInfoVisitor::recoverNonTransChar(parser::TStringNonTransChar *node)
{ return node->literal[0]; }

char
TokenInfoVisitor::recoverTransChar(parser::TStringTransChar *node)
{
    switch (node->literal[1]) {
        case 'a':   return '\a';
        case 'b':   return '\b';
        case 'f':   return '\f';
        case 'n':   return '\n';
        case 'r':   return '\r';
        case 't':   return '\t';
        case 'v':   return '\v';
        case '\\':  return '\\';
        case '\'':  return '\'';
        case '"':   return '\"';
        case '?':   return '\?';
        case 'x': {
            char ret = 0;
            char lower = std::tolower(node->literal[2]);
            ret += (lower > '9') ? (lower - 'a' + 10) : (lower - '0');
            lower = std::tolower(node->literal[3]);
            ret <<= 4;
            ret += (lower > '9') ? (lower - 'a' + 10) : (lower - '0');
            return ret;
        }
        default: {
            char ret = 0;
            ret += (node->literal[1] - 8);  ret <<= 3;
            ret += (node->literal[2] - 8);  ret <<= 3;
            ret += (node->literal[3] - 8);
            return ret;
        }
    }
}

char
TokenInfoVisitor::recoverChar(parser::TStringChar *node)
{
    assert(node->string_trans_char || node->string_non_trans_char);

    if (node->string_non_trans_char) {
        return recoverNonTransChar(node->string_non_trans_char.get());
    }
    else {
        return recoverTransChar(node->string_trans_char.get());
    }
}

std::string
TokenInfoVisitor::recoverString(parser::TString *node)
{
    std::string ret;

    for (auto &rep : node->string_rep_1) {
        ret += recoverChar(rep.get());
    }

    return ret;
}

std::string
TokenInfoVisitor::stringTransCharToString(parser::TStringTransChar *node)
{ return node->literal; }

std::string
TokenInfoVisitor::stringNonTransCharToString(parser::TStringNonTransChar *node)
{ return node->literal; }

std::string
TokenInfoVisitor::stringCharToString(parser::TStringChar *node)
{
    if (node->string_non_trans_char) {
        return stringNonTransCharToString(node->string_non_trans_char.get());
    }
    else {
        return stringTransCharToString(node->string_trans_char.get());
    }
}

std::string
TokenInfoVisitor::regexRangeToString(parser::TRegexRange *node)
{
    assert(node->getRule() > 0);
    assert(node->getRule() < 3);
    switch (node->getRule()) {
        case 1: return stringCharToString(
                        dynamic_cast<parser::TRegexRange_Rule1*>(node)->string_char.get());
        case 2: return stringCharToString(
                    dynamic_cast<parser::TRegexRange_Rule2*>(node)->string_char_1.get()) + '-'
                    + stringCharToString(
                    dynamic_cast<parser::TRegexRange_Rule2*>(node)->string_char_2.get());
        default: assert(false);
    }

    return "";
}

std::string
TokenInfoVisitor::regexUnaryToString(parser::TRegexUnary *node)
{
    assert(node->getRule() > 0);
    assert(node->getRule() < 4);
    switch (node->getRule()) {
        case 1: return std::string() + stringCharToString(
                        dynamic_cast<parser::TRegexUnary_Rule1*>(node)->string_char.get());
        case 2: return std::string("(") + regexContentToString(
                        dynamic_cast<parser::TRegexUnary_Rule2*>(node)->regex_content.get())
                        + ')';
        case 3: {
            std::string ret = "[";
            parser::TRegexUnary_Rule3 *rule = dynamic_cast<parser::TRegexUnary_Rule3*>(node);
            if (rule->regex_neg_opt) {
                ret += '^';
            }
            for (auto &rep : rule->regex_range_plus) {
                ret += regexRangeToString(rep.get());
            }
            return ret + ']';
        }
        default: assert(false);
    }

    return "";
}

std::string
TokenInfoVisitor::regexPartToString(parser::TRegexPart *node)
{
    std::string ret = regexUnaryToString(node->regex_unary.get());
    if (node->regex_repeatition_opt) {
        ret += node->regex_repeatition_opt->literal;
    }
    return ret;
}

std::string
TokenInfoVisitor::regexBranchToString(parser::TRegexBranch *node)
{
    std::string ret;
    for (auto &plus : node->regex_part_plus) {
        ret += regexPartToString(plus.get());
    }
    return ret;
}

std::string
TokenInfoVisitor::regexContentToString(parser::TRegexContent *node)
{
    std::string ret = regexBranchToString(node->regex_branch.get());
    for (auto &rep : node->regex_content_rep_1) {
        ret += '|' + regexBranchToString(rep.regex_branch.get());
    }
    return ret;
}

std::string
TokenInfoVisitor::regexToString(parser::TRegex *node)
{ return regexContentToString(node->regex_content.get()); }

void
TokenInfoVisitor::visit(parser::TokenRule_Rule1 *node)
{ _regex_token_table.emplace_back((node->id->literal == "_" ? "_WHITE" : node->id->literal), node->regex.get()); }

std::unique_ptr<FA>
TokenInfoVisitor::lexerFA()
{
    std::unique_ptr<FA> fa(new FA());

    fa->createState();  // start state is always 0

    int accept = TOKEN_START;

    for (auto token : _regex_token_table) {
        insertRegexIntoFa(fa.get(), token.regex, accept++);
    }

    for (int i = 1; i < 256; ++i) {
        fa->addLink(fa->createState(i), 0, i);
    }

    return fa;
}

int
TokenInfoVisitor::insertRegexIntoFa(FA *fa, parser::TRegex *regex, int accept)
{
    int accept_state = insertRegexContentIntoFa(fa, regex->regex_content.get(), 0);
    fa->getState(accept_state).accept = accept;
    return accept_state;
}

int
TokenInfoVisitor::insertRegexContentIntoFa(FA *fa, parser::TRegexContent *regex_content, int start)
{
    int accept_state = fa->createState();

    int state = insertRegexBranchIntoFa(fa, regex_content->regex_branch.get(), start);
    fa->addLink(accept_state, state, FA::EMPTY_LINK);

    for (auto &rep : regex_content->regex_content_rep_1) {
        state = insertRegexBranchIntoFa(fa, rep.regex_branch.get(), start);
        fa->addLink(accept_state, state, FA::EMPTY_LINK);
    }

    return accept_state;
}

int
TokenInfoVisitor::insertRegexBranchIntoFa(FA *fa, parser::TRegexBranch *regex_branch, int start)
{
    for (auto &plus : regex_branch->regex_part_plus) {
        start = insertRegexPartIntoFa(fa, plus.get(), start);
    }
    return start;
}

int
TokenInfoVisitor::insertRegexPartIntoFa(FA *fa, parser::TRegexPart *regex_part, int start)
{
    int unary_start = fa->createState();
    fa->addLink(unary_start, start, FA::EMPTY_LINK);
    int unary_accept = insertRegexUnaryIntoFa(fa, regex_part->regex_unary.get(), unary_start);
    int accept;
    if (regex_part->regex_repeatition_opt) {
        if (regex_part->regex_repeatition_opt->literal == "*") {
            accept = fa->createState();
            fa->addLink(unary_start, unary_accept, FA::EMPTY_LINK);
            fa->addLink(accept, start, FA::EMPTY_LINK);
            fa->addLink(accept, unary_accept, FA::EMPTY_LINK);
        }
        else if (regex_part->regex_repeatition_opt->literal == "+") {
            accept = fa->createState();
            unary_start = unary_accept;
            unary_accept = insertRegexUnaryIntoFa(fa, regex_part->regex_unary.get(), unary_start);
            fa->addLink(unary_start, unary_accept, FA::EMPTY_LINK);
            fa->addLink(accept, unary_accept, FA::EMPTY_LINK);
        }
        else { // ?
            accept = fa->createState();
            fa->addLink(accept, start, FA::EMPTY_LINK);
        }
        return accept;
    }
    else {
        return unary_accept;
    }
}

int
TokenInfoVisitor::insertRegexUnaryIntoFa(FA *fa, parser::TRegexUnary *regex_unary, int start)
{
    switch (regex_unary->getRule()) {
        case 1: {
            int accept_state = fa->createState();
            fa->addLink(accept_state, start, recoverChar(
                        dynamic_cast<parser::TRegexUnary_Rule1*>(regex_unary)->string_char.get()));
            return accept_state;
        }
        case 2: {
            return insertRegexContentIntoFa(fa, 
                        dynamic_cast<parser::TRegexUnary_Rule2*>(regex_unary)->regex_content.get(), start);
        }
        case 3: {
            int accept_state = fa->createState();
            auto *rule = dynamic_cast<parser::TRegexUnary_Rule3*>(regex_unary);

            std::set<char> char_set;
            for (auto &plus : rule->regex_range_plus) {
                if (plus->getRule() == 1) {
                    char_set.insert(recoverChar(dynamic_cast<parser::TRegexRange_Rule1*>(plus.get())->string_char.get()));
                }
                else {
                    int ch1 = recoverChar(dynamic_cast<parser::TRegexRange_Rule2*>(plus.get())->string_char_1.get());
                    int ch2 = recoverChar(dynamic_cast<parser::TRegexRange_Rule2*>(plus.get())->string_char_2.get());

                    for (int i = ch1; i <= ch2; ++i) {
                        char_set.insert(i);
                    }
                }
            }

            if (rule->regex_neg_opt) {
                for (int i = 0; i < 256; ++i) {
                    if (char_set.find(i) == char_set.end()) {
                        fa->addLink(accept_state, start, i);
                    }
                }
            }
            else {
                for (int i = 0; i < 256; ++i) {
                    if (char_set.find(i) != char_set.end()) {
                        fa->addLink(accept_state, start, i);
                    }
                }
            }

            return accept_state;
        }
    }

    assert(false);
}
