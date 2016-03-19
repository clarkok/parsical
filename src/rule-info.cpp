#include "rule-info.hpp"
#include "token-info.hpp"

using namespace parsical;

void
RuleInfoVisitor::visit(parser::SentenceRule *node)
{
    std::pair<int, int> range;

    range.first = _rule_list.size();
    appendRule(node->id->literal, node->sentence_decl->sentence_branch.get());

    for (auto &rep : node->sentence_decl->sentence_decl_rep_1) {
        appendRule(node->id->literal, rep.sentence_branch.get());
    }
    range.second = _rule_list.size();
    _rule_range.emplace(node->id->literal, range);
}

void
RuleInfoVisitor::visit(parser::TString *node)
{
    std::string original = "\"" + TokenInfoVisitorBase::recoverString(node) + "\"";
    std::pair<int, int> range;
    range.first = _rule_list.size();
    appendRule(original, nullptr);
    range.second = _rule_list.size();
    _rule_range.emplace(original, range);
}

void
RuleInfoVisitor::appendRule(std::string name, parser::SentenceBranch *rule)
{ _rule_list.emplace_back(_rule_list.size(), name, rule); }
