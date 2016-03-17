#include "lexer.hpp"

using namespace parsical;

void
Lexer::outputTokenEnum(std::ostream &_os)
{
    auto regex_iter = _token_info.begin(),
         regex_limit = _token_info.end();

    _os << "\t" << regex_iter->name << " = " << TokenInfoVisitor::TOKEN_START
        << "\t/* " << TokenInfoVisitor::regexToString(regex_iter->regex) << " */";

    ++regex_iter;

    while (regex_iter != regex_limit) {
        _os << "," << std::endl << "\t" << regex_iter->name << "\t/* " 
            << TokenInfoVisitor::regexToString(regex_iter->regex) << " */";
        ++regex_iter;
    }
}

void
Lexer::outputNextStateTable(std::ostream &_os)
{
    _os << "\tstatic const int next_state[" << _dfa->stateCount() << "][256] = {" << std::endl;
    for (auto &st : *_dfa) {
        _os << "\t\t{";
        for (int i = 0; i < 255; ++i) {
            if (!(i % 16)) {
                _os << std::endl << "\t\t\t";
            }
            _os << (int)((st.links.find(i) == st.links.end()) ? -1 : st.links.find(i)->second) << ",\t";
        }
        _os << (int)((st.links.find(255) == st.links.end()) ? -1 : st.links.find(255)->second) 
            << std::endl << "\t\t}," << std::endl;
    }
    _os << "\t};" << std::endl << std::endl;
}

void
Lexer::outputAcceptTable(std::ostream &_os)
{
    _os << "\tstatic const int accept_state[" << _dfa->stateCount() << "] = {";
    int counter = 0;
    for (auto &st : *_dfa) {
        if ((counter++ % 16) == 0) {
            _os << std::endl << "\t\t";
        }
        _os << (int)(st.accept ? st.accept : -1) << ",\t";
    }
    _os << std::endl << "\t};" << std::endl << std::endl;
}

void
Lexer::outputFATables(std::ostream &_os)
{
    outputNextStateTable(_os);
    outputAcceptTable(_os);
}

void
Lexer::outputNFA(std::ostream &_os)
{ _token_info.lexerFA()->output(_os); }

void
Lexer::outputDFA(std::ostream &_os)
{ _dfa->output(_os); }
