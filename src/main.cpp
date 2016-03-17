#include <iostream>

#include "parsical-parser.hpp"
#include "print-visitor.hpp"
#include "token-info.hpp"

using namespace std;

int
main(int argc, char **argv)
{
    --argc; ++argv;
    if (!argc) {
        std::cout << "No input file" << std::endl;
        return -1;
    }

    parsical::parser::ParsicalParser pp;
    parsical::TokenInfoVisitor token_info;
    parsical::PrintVisitor pv(std::cout);

    auto *ast = pp.parse_file(*argv);

    ast->accept(&token_info);

    for (auto &token : token_info) {
        std::cout << token.name << '\t';
        if (token.content->getType() == parsical::parser::N_STRING) {
            std::cout << parsical::TokenInfoVisitor::recoverString(
                    dynamic_cast<parsical::parser::TString*>(token.content));
        }
        else {
            std::cout << parsical::TokenInfoVisitor::regexToString(
                    dynamic_cast<parsical::parser::TRegex*>(token.content));
        }
        std::cout << std::endl;
    }

    return 0;
}
