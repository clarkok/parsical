#include "../parsical-parser.hpp"
#include "parse_string.hpp"

namespace {

using namespace parsical::parser;

TRegexRepeatition *
parse_regex_repeatition(Location location, std::string::iterator &iter)
{
    auto begin = iter++;
    return new TRegexRepeatition(
            location,
            std::string(begin, iter)
        );
}

TRegexRange *
parse_regex_range(Location location, std::string::iterator &iter)
{
    auto *string_char_1 = parse_string_char(location, iter);

    if (*iter == '-') {
        ++iter;

        return new TRegexRange_Rule2(
                location,
                string_char_1,
                parse_string_char(location, iter)
            );
    }
    else {
        return new TRegexRange_Rule1(
                location,
                string_char_1
            );
    }
}

TRegexNeg *
parse_regex_neg(Location location, std::string::iterator &iter)
{
    auto begin = iter++;
    return new TRegexNeg(
            location,
            std::string(begin, iter)
        );
}

TRegexContent *
parse_regex_content(Location location, std::string::iterator &iter);

TRegexUnary *
parse_regex_unary(Location location, std::string::iterator &iter)
{
    if (*iter == '(') {
        ++iter;
        auto *regex_content = parse_regex_content(location, iter);
        ++iter;
        return new TRegexUnary_Rule2(
                location,
                regex_content
            );
    }
    else if (*iter == '[') {
        ++iter;
        TRegexNeg *regex_neg_opt = nullptr;
        if (*iter == '^') {
            regex_neg_opt = parse_regex_neg(location, iter);
        }

        auto *regex_unary = new TRegexUnary_Rule3(location, regex_neg_opt);

        while (*iter != ']') {
            regex_unary->regex_range_plus.emplace_back(parse_regex_range(location, iter));
        }
        ++iter;
        return regex_unary;
    }
    else {
        return new TRegexUnary_Rule1(
                location,
                parse_string_char(location, iter)
            );
    }
}

TRegexPart *
parse_regex_part(Location location, std::string::iterator &iter)
{
    auto *regex_unary = parse_regex_unary(location, iter);
    TRegexRepeatition *regex_repeatition_opt = nullptr;

    switch (*iter) {
        case '*':
        case '+':
        case '?':
            regex_repeatition_opt = parse_regex_repeatition(location, iter);
    }

    return new TRegexPart(
            location,
            regex_unary,
            regex_repeatition_opt
        );
}

TRegexBranch *
parse_regex_branch(Location location, std::string::iterator &iter)
{
    auto *ret = new TRegexBranch(location);

    while (*iter != '|' && *iter != '/' && *iter != ')') {
        ret->regex_part_plus.emplace_back(parse_regex_part(location, iter));
    }

    return ret;
}

TRegexContent *
parse_regex_content(Location location, std::string::iterator &iter)
{
    auto *ret = new TRegexContent(
            location,
            parse_regex_branch(location, iter)
        );

    while (*iter == '|') {
        ++iter;
        TRegexContent::TRegexContentRep1 rep;
        rep.regex_branch.reset(parse_regex_branch(location, iter));
        ret->regex_content_rep_1.emplace_back(std::move(rep));
    }

    return ret;
}

TRegex *
parse_regex(Location location, std::string literal)
{
    auto iter = literal.begin() + 1;

    return new TRegex(
            location,
            parse_regex_content(location, iter)
        );
}

}
