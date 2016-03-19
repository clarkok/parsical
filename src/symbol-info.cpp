#include "report.hpp"
#include "symbol-info.hpp"

using namespace parsical;

void
SymbolInfoVisitor::reportDuplicated(parser::TId *met, parser::TreeNode *prev)
{
    Report::getReport().error(
            Report::positionedMessage(
                met->location,
                "duplicated symbol: `" + met->literal + "\'",
                "original defined at: " + Report::locationToString(prev->location)
            )
        );
}

void
SymbolInfoVisitor::reportMissing(parser::TId *met)
{
    Report::getReport().error(
            Report::positionedMessage(
                met->location,
                "met undefined symbol: `" + met->literal + "\'"
            )
        );
}

void
SymbolInfoVisitor::visit(parser::TokenRule_Rule1 *node)
{
    if (_symbol_table.find(node->id->literal) != _symbol_table.end()) {
        reportDuplicated(node->id.get(), _symbol_table[node->id->literal]);
    }

    _symbol_name[_symbol_id_counter] = node->id->literal;
    _symbol_id[node->id->literal] = _symbol_id_counter++;

    _symbol_table[node->id->literal] = node->regex.get();
}

void
SymbolInfoVisitor::visit(parser::TokenRule_Rule2 *node)
{
    if (_symbol_table.find(node->id->literal) != _symbol_table.end()) {
        reportDuplicated(node->id.get(), _symbol_table[node->id->literal]);
    }

    _symbol_name[_symbol_id_counter] = node->id->literal;
    _symbol_id[node->id->literal] = _symbol_id_counter++;

    _symbol_table[node->id->literal] = node->sentence_decl.get();
}

void
SymbolInfoVisitor::visit(parser::TokenRule_Rule3 *node)
{
    if (_fragment_table.find(node->id->literal) != _fragment_table.end()) {
        reportDuplicated(node->id.get(), _fragment_table[node->id->literal]);
    }

    if (_symbol_id.find(node->id->literal) == _symbol_id.end()) {
        _symbol_name[_symbol_id_counter] = node->id->literal;
        _symbol_id[node->id->literal] = _symbol_id_counter++;
    }

    _fragment_table[node->id->literal] = node->regex.get();
}

void
SymbolInfoVisitor::visit(parser::SentenceRule *node)
{
    if (_symbol_table.find(node->id->literal) != _symbol_table.end()) {
        reportDuplicated(node->id.get(), _symbol_table[node->id->literal]);
    }

    _symbol_name[_symbol_id_counter] = node->id->literal;
    _symbol_id[node->id->literal] = _symbol_id_counter++;

    _symbol_table[node->id->literal] = node->sentence_decl.get();
}
