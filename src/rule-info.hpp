#ifndef _PARSICAL_RULE_INFO_HPP_
#define _PARSICAL_RULE_INFO_HPP_

#include <vector>
#include <string>
#include <map>

#include "parsical-parser.hpp"

namespace parsical {

struct Rule
{
    int rule_id;
    std::string name;
    parser::SentenceBranch *branch;

    Rule(int rule_id, std::string name, parser::SentenceBranch *branch)
        : rule_id(rule_id), name(name), branch(branch)
    { }
};

class RuleInfoVisitor : public parser::Visitor
{
    std::vector<Rule> _rule_list;
    std::map<std::string, std::pair<int, int> > _rule_range;

    void appendRule(std::string name, parser::SentenceBranch *rule);
public:
    RuleInfoVisitor() = default;
    virtual ~RuleInfoVisitor() = default;

    inline Rule &
    getRule(int index)
    { return _rule_list[index]; }

    inline std::pair<int, int> &
    getRange(std::string name)
    { return _rule_range[name]; }

    inline bool
    has(std::string name)
    { return _rule_range.find(name) != _rule_range.end(); }

    define_visitor_visit(parser::SentenceRule)
    define_visitor_visit(parser::TString)
};

}

#endif
