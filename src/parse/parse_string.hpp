#include "../parsical-parser.hpp"

namespace {

using namespace parsical::parser;

TStringTransChar *
parse_string_trans_char(Location location, std::string::iterator &iter)
{
    auto begin = iter;
    ++iter;

    switch (*iter) {
        case 'a':   case 'b':   case 'f':   case 'n':
        case 'r':   case 't':   case 'v':   case '\\':
        case '\'':  case '"':   case '?':   case '/':
            return new TStringTransChar(location, std::string(begin, ++iter));
        default:
            return new TStringTransChar(location, std::string(begin, iter += 3));
    }
}

TStringNonTransChar *
parse_string_non_trans_char(Location location, std::string::iterator &iter)
{
    auto begin = iter++;
    return new TStringNonTransChar(location, std::string(begin, iter));
}

TStringChar *
parse_string_char(Location location, std::string::iterator &iter)
{
    if (*iter == '\\') {
        return new TStringChar(location, nullptr, parse_string_trans_char(location, iter));
    }
    else {
        return new TStringChar(location, parse_string_non_trans_char(location, iter), nullptr);
    }
}

TString *
parse_string(Location location, std::string literal)
{
    auto iter = literal.begin() + 1;
    auto *ret = new TString(location);

    while (*iter != '"') {
        ret->string_rep_1.emplace_back(parse_string_char(location, iter));
    }

    return ret;
}

}
