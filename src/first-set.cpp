#include "first-set.hpp"
#include "token-info.hpp"

using namespace parsical;

void
FirstSetVisitor::visit(parser::SentenceRule *node)
{
    if (_first_set.find(node->id->literal) != _first_set.end()) return;
    calculateSentence(node->id->literal, node->sentence_decl.get());
}

void
FirstSetVisitor::calculateSentence(std::string name, parser::SentenceDecl *sentence_decl)
{
    _first_set.emplace(name, CharSet());

    CharSet result = calculateSentenceBranch(sentence_decl->sentence_branch.get());
    for (auto &rep : sentence_decl->sentence_decl_rep_1) {
        CharSet set = calculateSentenceBranch(rep.sentence_branch.get());
        result.insert(set.begin(), set.end());
    }

    _first_set[name].insert(result.begin(), result.end());
}

CharSet
FirstSetVisitor::calculateSentenceBranch(parser::SentenceBranch *sentence_branch)
{
    CharSet ret;

    auto iter = sentence_branch->sentence_part_rep.begin();

    while (iter != sentence_branch->sentence_part_rep.end()) {
        CharSet set = calculateSentencePart(iter->get());
        ret.erase(EMPTY_CHAR);
        ret.insert(set.begin(), set.end());

        if (ret.find(EMPTY_CHAR) == ret.end()) { return ret; }
        ++iter;
    }

    return ret;
}

CharSet
FirstSetVisitor::calculateSentencePart(parser::SentencePart *sentence_part)
{
    CharSet ret(calculateSentenceUnary(sentence_part->sentence_unary.get()));

    if (sentence_part->sentence_repeatition_opt) {
        if (sentence_part->sentence_repeatition_opt->getRule() != 2) {
            ret.insert(EMPTY_CHAR);
        }
    }

    return ret;
}

CharSet
FirstSetVisitor::calculateSentenceUnary(parser::SentenceUnary *node)
{
    switch (node->getRule()) {
        case 1: {
            auto *rule = dynamic_cast<parser::SentenceUnary_Rule1*>(node);
            if (_first_set.find(rule->id->literal) == _first_set.end()) {
                calculateSentence(
                        rule->id->literal,
                        dynamic_cast<parser::SentenceDecl*>(_symbols->findSymbol(rule->id->literal))
                    );
            }
            return _first_set[rule->id->literal];
        }
        case 2: {
            auto *rule = dynamic_cast<parser::SentenceUnary_Rule2*>(node);
            return {TokenInfoVisitorBase::recoverChar(rule->string->string_rep_1.front().get())};
        }
        case 3: {
            return {'('};
        }
    }
    assert(false);
    return CharSet();
}
