#ifndef _PARSICAL_LEXER_HPP_
#define _PARSICAL_LEXER_HPP_

#include <vector>
#include <map>

#include "parsical-parser.hpp"

namespace parsical
{

struct Token
{
    std::string name;
    parser::TreeNode *content;

    Token(std::string name, parser::TreeNode *content)
        : name(name), content(content)
    { }
};

class TokenInfoVisitor : public parser::Visitor
{
    std::vector<Token>          _token_table;
    std::map<std::string, int>  _string_literal_map;
    int _annoymous_counter;

public:
    static char recoverNonTransChar(parser::TStringNonTransChar *node);
    static char recoverTransChar(parser::TStringTransChar *node);
    static char recoverChar(parser::TStringChar *node);
    static std::string recoverString(parser::TString *node);

    static std::string regexRangeToString(parser::TRegexRange *node);
    static std::string regexUnaryToString(parser::TRegexUnary *node);
    static std::string regexPartToString(parser::TRegexPart *node);
    static std::string regexBranchToString(parser::TRegexBranch *node);
    static std::string regexContentToString(parser::TRegexContent *node);
    static std::string regexToString(parser::TRegex *node);

    TokenInfoVisitor()
        : _annoymous_counter(0)
    { }

    virtual ~TokenInfoVisitor() = default;

    define_visitor_visit(parser::TokenRule_Rule1)
    define_visitor_visit(parser::TString)

    auto begin() -> decltype(TokenInfoVisitor::_token_table.begin())
    { return _token_table.begin(); }

    auto end() -> decltype(TokenInfoVisitor::_token_table.end())
    { return _token_table.end(); }
};

};

#endif
