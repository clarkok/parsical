#ifndef _PARSICAL_LEXER_HPP_
#define _PARSICAL_LEXER_HPP_

#include <iostream>
#include <memory>

#include "parsical-parser.hpp"
#include "token-info.hpp"
#include "fa.hpp"

namespace parsical {

class Lexer
{
    TokenInfoVisitor _token_info;
    std::unique_ptr<FA> _dfa;

    void outputNextStateTable(std::ostream &_os);
    void outputAcceptTable(std::ostream &_os);

public:
    Lexer(parser::Grammar *grammar)
    {
        grammar->accept(&_token_info);
        _dfa = _token_info.lexerFA()->nfa2dfa();
    }

    ~Lexer() = default;

    void outputTokenEnum(std::ostream &_os);
    void outputFATables(std::ostream &_os);
    void outputNFA(std::ostream &_os);
    void outputDFA(std::ostream &_os);
};

}

#endif
