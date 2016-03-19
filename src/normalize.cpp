#include <cassert>

#include "normalize.hpp"

using namespace parsical;

void
NormalizeVisitor::visit(parser::SentenceRule *node)
{
    _current = node;
    _counter = 0;
    parser::Visitor::visit(node);
}

void
NormalizeVisitor::visit(parser::TokenRule_Rule2 *node)
{
    _current = node;
    _counter = 0;
    parser::Visitor::visit(node);
}

void
NormalizeVisitor::visit(parser::TokenRule_Rule4 *node)
{
    _current = node;
    _counter = 0;
    parser::Visitor::visit(node);
}

void
NormalizeVisitor::visit(parser::SentencePart *node)
{
    if (node->sentence_unary->getRule() == 3) {
        assert(_current->getType() == parser::N_sentence_rule ||
               _current->getType() == parser::N_token_rule);
        ++_counter;

        if (_current->getType() == parser::N_sentence_rule) {
            parser::Grammar::GrammarRep1 rep;
            rep.rule.reset(
                new parser::Rule_Rule2(
                    node->sentence_unary->location,
                    new parser::SentenceRule(
                        node->sentence_unary->location,
                        new parser::TId(
                            node->sentence_unary->location,
                            dynamic_cast<parser::SentenceRule*>(_current)->id->literal
                            + "$Part" + std::to_string(_counter)
                        ),
                        new parser::SentenceDecl(
                            node->sentence_unary->location,
                            dynamic_cast<parser::SentenceUnary_Rule3*>(
                                node->sentence_unary.get())->sentence_branch.release()
                        )
                    )
                )
            );
            _grammar->grammar_rep_1.emplace_back(std::move(rep));

            node->sentence_unary.reset(
                new parser::SentenceUnary_Rule1(
                    node->sentence_unary->location,
                    new parser::TId(
                        node->sentence_unary->location,
                        dynamic_cast<parser::SentenceRule*>(_current)->id->literal
                        + "$Part" + std::to_string(_counter)
                    )
                )
            );
        }
        else if (dynamic_cast<parser::TokenRule*>(_current)->getRule() == 2) {
            parser::Grammar::GrammarRep1 rep;
            rep.rule.reset(
                new parser::Rule_Rule1(
                    node->sentence_unary->location,
                    new parser::TokenRule_Rule2(
                        node->sentence_unary->location,
                        new parser::TId(
                            node->sentence_unary->location,
                            dynamic_cast<parser::TokenRule_Rule2*>(_current)->id->literal
                            + "$Part" + std::to_string(_counter)
                        ),
                        new parser::SentenceDecl(
                            node->sentence_unary->location,
                            dynamic_cast<parser::SentenceUnary_Rule3*>(
                                node->sentence_unary.get())->sentence_branch.release()
                        )
                    )
                )
            );
            _grammar->grammar_rep_1.emplace_back(std::move(rep));

            node->sentence_unary.reset(
                new parser::SentenceUnary_Rule1(
                    node->sentence_unary->location,
                    new parser::TId(
                        node->sentence_unary->location,
                        dynamic_cast<parser::TokenRule_Rule2*>(_current)->id->literal
                        + "$Part" + std::to_string(_counter)
                    )
                )
            );
        }
        else {
            parser::Grammar::GrammarRep1 rep;
            rep.rule.reset(
                new parser::Rule_Rule1(
                    node->sentence_unary->location,
                    new parser::TokenRule_Rule4(
                        node->sentence_unary->location,
                        new parser::TId(
                            node->sentence_unary->location,
                            dynamic_cast<parser::TokenRule_Rule4*>(_current)->id->literal
                            + "$Part" + std::to_string(_counter)
                        ),
                        new parser::SentenceDecl(
                            node->sentence_unary->location,
                            dynamic_cast<parser::SentenceUnary_Rule3*>(
                                node->sentence_unary.get())->sentence_branch.release()
                        )
                    )
                )
            );
            _grammar->grammar_rep_1.emplace_back(std::move(rep));

            node->sentence_unary.reset(
                new parser::SentenceUnary_Rule1(
                    node->sentence_unary->location,
                    new parser::TId(
                        node->sentence_unary->location,
                        dynamic_cast<parser::TokenRule_Rule4*>(_current)->id->literal
                        + "$Part" + std::to_string(_counter)
                    )
                )
            );
        }
    }
}
