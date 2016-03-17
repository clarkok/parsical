#ifndef _PARSICAL_TOKEN_INFO_HPP_
#define _PARSICAL_TOKEN_INFO_HPP_

#include <vector>
#include <map>
#include <memory>

#include "parsical-parser.hpp"
#include "fa.hpp"

namespace parsical
{

struct StringToken
{
    std::string name;
    parser::TString *string;

    StringToken(std::string name, parser::TString *string)
        : name(name), string(string)
    { }
};

struct RegexToken
{
    std::string name;
    parser::TRegex *regex;

    RegexToken(std::string name, parser::TRegex *regex)
        : name(name), regex(regex)
    { }
};

class TokenInfoVisitor : public parser::Visitor
{
    std::vector<RegexToken>     _regex_token_table;

    static char recoverNonTransChar(parser::TStringNonTransChar *node);
    static char recoverTransChar(parser::TStringTransChar *node);
    static char recoverChar(parser::TStringChar *node);

    static std::string stringTransCharToString(parser::TStringTransChar *node);
    static std::string stringNonTransCharToString(parser::TStringNonTransChar *node);
    static std::string stringCharToString(parser::TStringChar *node);
    static std::string regexRangeToString(parser::TRegexRange *node);
    static std::string regexUnaryToString(parser::TRegexUnary *node);
    static std::string regexPartToString(parser::TRegexPart *node);
    static std::string regexBranchToString(parser::TRegexBranch *node);
    static std::string regexContentToString(parser::TRegexContent *node);

    int insertRegexIntoFa(FA *fa, parser::TRegex *regex, int accept);

    int insertRegexContentIntoFa(FA *fa, parser::TRegexContent *regex_content, int start);
    int insertRegexBranchIntoFa(FA *fa, parser::TRegexBranch *regex_branch, int start);
    int insertRegexPartIntoFa(FA *fa, parser::TRegexPart *regex_part, int start);
    int insertRegexUnaryIntoFa(FA *fa, parser::TRegexUnary *regex_unary, int start);

public:
    static std::string recoverString(parser::TString *node);
    static std::string regexToString(parser::TRegex *node);

    static const int TOKEN_START = 256;

    TokenInfoVisitor() = default;

    virtual ~TokenInfoVisitor() = default;

    define_visitor_visit(parser::TokenRule_Rule1)

    auto begin() -> decltype(TokenInfoVisitor::_regex_token_table.begin())
    { return _regex_token_table.begin(); }

    auto end() -> decltype(TokenInfoVisitor::_regex_token_table.end())
    { return _regex_token_table.end(); }

    std::unique_ptr<FA> lexerFA();
};

};

#endif
