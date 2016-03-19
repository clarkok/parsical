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
    std::unique_ptr<TokenInfoVisitorBase> _token_info;
    std::unique_ptr<FA> _dfa;

    void outputNextStateTable(std::ostream &_os);
    void outputAcceptTable(std::ostream &_os);

public:
    Lexer(parser::Grammar *grammar)
        : _token_info(new TokenInfoVisitor())
    {
        grammar->accept(_token_info.get());
        _dfa = _token_info->lexerFA()->nfa2dfa();
    }

    Lexer(TokenInfoVisitorBase *token_info)
        : _token_info(token_info)
    { _dfa = _token_info->lexerFA()->nfa2dfa(); }

    ~Lexer() = default;

    void outputTokenEnum(std::ostream &_os);
    void outputFATables(std::ostream &_os);
    void outputNFA(std::ostream &_os);
    void outputDFA(std::ostream &_os);
};

}

#endif
