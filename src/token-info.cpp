#include <cassert>
#include <iostream>

#include "token-info.hpp"

using namespace parsical;

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
TokenInfoVisitor::regexRangeToString(parser::TRegexRange *node)
{
    assert(node->getRule() > 0);
    assert(node->getRule() < 3);
    switch (node->getRule()) {
        case 1: return std::string() + recoverChar(
                        dynamic_cast<parser::TRegexRange_Rule1*>(node)->string_char.get());
        case 2: return std::string() + recoverChar(
                    dynamic_cast<parser::TRegexRange_Rule2*>(node)->string_char_1.get()) + '-'
                    + recoverChar(
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
        case 1: return std::string() + recoverChar(
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
{ _token_table.emplace_back(node->id->literal, node->regex.get()); }

void
TokenInfoVisitor::visit(parser::TString *node)
{
    std::string original = recoverString(node);

    if (_string_literal_map.find(original) == _string_literal_map.end()) {
        _string_literal_map[original] = _token_table.size();
        _token_table.emplace_back("T__" + std::to_string(_annoymous_counter++), node);
    }
}
