#include <iostream>

#include "pegtl/pegtl.hh"

#include "parsical-parser.hpp"

namespace {

struct _grammar;
struct _rule;
struct _token_rule;
struct _sentence_rule;
struct _sentence_decl;
struct _sentence_branch;
struct _sentence_part;
struct _sentence_unary;
struct _sentence_decorate;
struct _sentence_repeatition;
struct _ID;
struct _STRING;
struct _STRING_CHAR;
struct _STRING_NON_TRANS_CHAR;
struct _STRING_TRANS_CHAR;
struct _REGEX;
struct _REGEX_CONTENT;
struct _REGEX_BRANCH;
struct _REGEX_PART;
struct _REGEX_UNARY;
struct _REGEX_REPEATITION;

struct WHITE:
    pegtl::star<
        pegtl::one<' ', '\t', '\v', '\r', '\n'>
    >
{ };

template <typename T>
struct Token:
    pegtl::seq<T, WHITE>
{ };

template <typename T>
struct Forward:
    pegtl::sor<T>
{ };

struct _REGEX_REPEATITION:
    pegtl::sor<
        pegtl::one<'*'>,
        pegtl::one<'+'>,
        pegtl::one<'?'>
    >
{ };

struct __REGEX_UNARY_RULE_1:
    Forward<_STRING_CHAR>
{ };

struct __REGEX_UNARY_RULE_2:
    pegtl::seq<
        pegtl::one<'('>,
        _REGEX_CONTENT,
        pegtl::one<')'>
    >
{ };

struct _REGEX_UNARY:
    pegtl::sor<
        __REGEX_UNARY_RULE_1,
        __REGEX_UNARY_RULE_2
    >
{ };

struct _REGEX_PART:
    pegtl::seq<
        _REGEX_UNARY,
        pegtl::opt<
            _REGEX_REPEATITION
        >
    >
{ };

struct __REGEX_BRANCH_REP_1:
    _REGEX_PART
{ };

struct _REGEX_BRANCH:
    pegtl::plus<
        __REGEX_BRANCH_REP_1
    >
{ };

struct __REGEX_CONTENT_REP_1:
    pegtl::seq<
        pegtl::one<'|'>,
        _REGEX_BRANCH
    >
{ };

struct _REGEX_CONTENT:
    pegtl::seq<
        _REGEX_BRANCH,
        pegtl::star<
            __REGEX_CONTENT_REP_1
        >
    >
{ };

struct _REGEX:
    pegtl::seq<
        pegtl::one<'/'>,
        _REGEX_CONTENT,
        pegtl::one<'/'>
    >
{ };

struct _STRING_TRANS_CHAR:
    pegtl::seq<
        pegtl::one<'\\'>,
        pegtl::sor<
            pegtl::one<'a', 'b', 'f', 'n', 'r', 't', 'v', '\\', '\'', '"', '?'>,
            pegtl::seq<
                pegtl::ranges<'0', '7'>,
                pegtl::ranges<'0', '7'>,
                pegtl::ranges<'0', '7'>
            >,
            pegtl::seq<
                pegtl::one<'x'>,
                pegtl::ranges<'0', '9', 'a', 'f', 'A', 'F'>,
                pegtl::ranges<'0', '9', 'a', 'f', 'A', 'F'>
            >
        >
    >
{ };

struct _STRING_NON_TRANS_CHAR:
    pegtl::not_one<'\\', '"'>
{ };

struct _STRING_CHAR:
    pegtl::sor<
        _STRING_NON_TRANS_CHAR,
        _STRING_TRANS_CHAR
    >
{ };

struct __STRING_REP_1:
    _STRING_CHAR
{ };

struct _STRING:
    pegtl::seq<
        pegtl::one<'"'>,
        pegtl::plus<__STRING_REP_1>,
        pegtl::one<'"'>
    >
{ };

struct _ID:
    pegtl::seq<
        pegtl::ranges<'a', 'z', 'A', 'Z'>,
        pegtl::star<
            pegtl::sor<pegtl::one<'_'>, pegtl::ranges<'a', 'z', 'A', 'Z', '0', '9'> >
        >
    >
{ };

struct __sentence_repeatition_RULE_1:
    Token<pegtl::one<'*'> >
{ };

struct __sentence_repeatition_RULE_2:
    Token<pegtl::one<'+'> >
{ };

struct __sentence_repeatition_RULE_3:
    Token<pegtl::one<'?'> >
{ };

struct _sentence_repeatition:
    pegtl::sor<
        __sentence_repeatition_RULE_1,
        __sentence_repeatition_RULE_2,
        __sentence_repeatition_RULE_3
    >
{ };

struct _sentence_decorate:
    pegtl::one<'!'>
{ };

struct __sentence_unary_RULE_1:
    Token<_ID>
{ };

struct __sentence_unary_RULE_2:
    Token<_STRING>
{ };

struct __sentence_unary_RULE_3:
    pegtl::seq<
        Token<pegtl::one<'('> >,
        Token<_sentence_branch>,
        Token<pegtl::one<')'> >
    >
{ };

struct _sentence_unary:
    pegtl::sor<
        __sentence_unary_RULE_1,
        __sentence_unary_RULE_2,
        __sentence_unary_RULE_3
    >
{ };

struct __sentence_part_REP_1:
    Token<_sentence_decorate>
{ };

struct __sentence_part_REP_2:
    Token<_sentence_repeatition>
{ };

struct _sentence_part:
    pegtl::seq<
        Token<_sentence_unary>,
        pegtl::opt<__sentence_part_REP_1>,
        pegtl::opt<__sentence_part_REP_2>
    >
{ };

struct __sentence_branch_REP_1:
    Token<_sentence_part>
{ };

struct _sentence_branch:
    pegtl::plus<__sentence_branch_REP_1>
{ };

struct __sentence_decl_REP_1:
    pegtl::seq<
        Token<pegtl::one<'|'> >,
        Token<_sentence_branch>
    >
{ };

struct _sentence_decl:
    pegtl::seq<
        Token<_sentence_branch>,
        pegtl::star<__sentence_decl_REP_1>
    >
{ };

struct _sentence_rule:
    pegtl::seq<
        Token<_ID>,
        Token<pegtl::one<':'> >,
        Token<_sentence_decl>
    >
{ };

struct __token_rule_RULE_1:
    pegtl::seq<
        Token<_ID>,
        Token<pegtl::one<'='> >,
        Token<_REGEX>
    >
{ };

struct __token_rule_RULE_2:
    pegtl::seq<
        Token<_ID>,
        Token<pegtl::one<'='> >,
        Token<_sentence_decl>
    >
{ };

struct _token_rule:
    pegtl::sor<
        __token_rule_RULE_1,
        __token_rule_RULE_2
    >
{ };

struct __rule_RULE_1:
    Token<_token_rule>
{ };

struct __rule_RULE_2:
    Token<_sentence_rule>
{ };

struct _rule:
    pegtl::sor<
        __rule_RULE_1,
        __rule_RULE_2
    >
{ };

struct __grammar_REP_1:
    pegtl::seq<
        Token<_rule>,
        Token<pegtl::one<';'> >
    >
{ };

struct _grammar:
    pegtl::plus<
        __grammar_REP_1
    >
{ };

struct Grammar:
    pegtl::seq<
        WHITE,
        _grammar
    >
{ };

}

void
parsical::Parser::ParsicalParser::parse(const std::string &filename)
{
    pegtl::file_parser fp(filename);

    std::cout << fp.parse<Grammar>() << std::endl;
}
