#include "print-visitor.hpp"

using namespace parsical;

#define print_visit(type)                                   \
    void                                                    \
    PrintVisitor::visit(type *node)                         \
    {                                                       \
        for (int i = 0; i < _indent; ++i) _os << "  ";      \
        _os << #type << std::endl;                          \
        ++_indent;                                          \
        parser::Visitor::visit(node);                       \
        --_indent;                                          \
    }

ast_each(print_visit)
