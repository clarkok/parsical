#ifndef __PARSICAL_PARSER_HPP__
#define __PARSICAL_PARSER_HPP__

#include <string>
#include <memory>
#include <list>
#include <stack>

namespace parsical {

namespace parser {

enum NodeType {
    N_TOKEN = 256,
    N_grammar,
    N_rule,
    N_token_rule,
    N_sentence_rule,
    N_sentence_decl,
    N_sentence_branch,
    N_sentence_part,
    N_sentence_unary,
    N_sentence_decorate,
    N_sentence_repeatition,
    N_ID,
    N_STRING,
    N_STRING_CHAR,
    N_STRING_NON_TRANS_CHAR,
    N_STRING_TRANS_CHAR,
    N_REGEX,
    N_REGEX_CONTENT,
    N_REGEX_BRANCH,
    N_REGEX_PART,
    N_REGEX_UNARY,
    N_REGEX_REPEATITION,
    N_REGEX_NEG,
    N_REGEX_RANGE,

    N_UNKNOWN
};

class Grammar;
class Rule;
class Rule_Rule1;
class Rule_Rule2;
class TokenRule;
class TokenRule_Rule1;
class TokenRule_Rule2;
class TokenRule_Rule3;
class TokenRule_Rule4;
class SentenceRule;
class SentenceDecl;
class SentenceBranch;
class SentencePart;
class SentenceUnary;
class SentenceUnary_Rule1;
class SentenceUnary_Rule2;
class SentenceUnary_Rule3;
class SentenceDecorate;
class SentenceRepeatition;
class SentenceRepeatition_Rule1;
class SentenceRepeatition_Rule2;
class SentenceRepeatition_Rule3;
class TId;
class TString;
class TStringChar;
class TStringNonTransChar;
class TStringTransChar;
class TRegex;
class TRegexContent;
class TRegexBranch;
class TRegexPart;
class TRegexUnary;
class TRegexUnary_Rule1;
class TRegexUnary_Rule2;
class TRegexUnary_Rule3;
class TRegexRange;
class TRegexRange_Rule1;
class TRegexRange_Rule2;
class TRegexRepeatition;
class TRegexNeg;

class TreeNode;
class TokenNode;

#define ast_each(macro)                                     \
    macro(parsical::parser::TreeNode)                       \
    macro(parsical::parser::TokenNode)                      \
    macro(parsical::parser::Grammar)                        \
    macro(parsical::parser::Rule)                           \
    macro(parsical::parser::Rule_Rule1)                     \
    macro(parsical::parser::Rule_Rule2)                     \
    macro(parsical::parser::TokenRule)                      \
    macro(parsical::parser::TokenRule_Rule1)                \
    macro(parsical::parser::TokenRule_Rule2)                \
    macro(parsical::parser::TokenRule_Rule3)                \
    macro(parsical::parser::TokenRule_Rule4)                \
    macro(parsical::parser::SentenceRule)                   \
    macro(parsical::parser::SentenceDecl)                   \
    macro(parsical::parser::SentenceBranch)                 \
    macro(parsical::parser::SentencePart)                   \
    macro(parsical::parser::SentenceUnary)                  \
    macro(parsical::parser::SentenceUnary_Rule1)            \
    macro(parsical::parser::SentenceUnary_Rule2)            \
    macro(parsical::parser::SentenceUnary_Rule3)            \
    macro(parsical::parser::SentenceDecorate)               \
    macro(parsical::parser::SentenceRepeatition)            \
    macro(parsical::parser::SentenceRepeatition_Rule1)      \
    macro(parsical::parser::SentenceRepeatition_Rule2)      \
    macro(parsical::parser::SentenceRepeatition_Rule3)      \
    macro(parsical::parser::TId)                            \
    macro(parsical::parser::TString)                        \
    macro(parsical::parser::TStringChar)                    \
    macro(parsical::parser::TStringNonTransChar)            \
    macro(parsical::parser::TStringTransChar)               \
    macro(parsical::parser::TRegex)                         \
    macro(parsical::parser::TRegexContent)                  \
    macro(parsical::parser::TRegexBranch)                   \
    macro(parsical::parser::TRegexPart)                     \
    macro(parsical::parser::TRegexUnary)                    \
    macro(parsical::parser::TRegexUnary_Rule1)              \
    macro(parsical::parser::TRegexUnary_Rule2)              \
    macro(parsical::parser::TRegexUnary_Rule3)              \
    macro(parsical::parser::TRegexRange)                    \
    macro(parsical::parser::TRegexRange_Rule1)              \
    macro(parsical::parser::TRegexRange_Rule2)              \
    macro(parsical::parser::TRegexRepeatition)              \
    macro(parsical::parser::TRegexNeg)

#define define_visitor_visit(type)                          \
    virtual void visit(type *);

#define define_visitor  ast_each(define_visitor_visit)

class Visitor
{
public:
    virtual ~Visitor() = default;

    define_visitor
};

struct Location
{
    std::string file;
    int line;
    int column;

    Location(std::string file, int line, int column)
        : file(file), line(line), column(column)
    { }
};

class TreeNode
{
public:
    Location location;

    TreeNode (Location location)
        : location(location)
    { }

    virtual ~TreeNode() = default;

    virtual int
    getType() const
    { return N_UNKNOWN; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TokenNode : public TreeNode
{
public:
    std::string literal;

    TokenNode (Location location, std::string literal)
        : TreeNode(location), literal(literal)
    { }

    virtual ~TokenNode() = default;

    virtual int
    getType() const
    { return N_TOKEN; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TRegexRepeatition : public TokenNode
{
public:
    TRegexRepeatition (Location location, std::string literal)
        : TokenNode(location, literal)
    { }

    virtual ~TRegexRepeatition() = default;

    virtual int
    getType() const
    { return N_REGEX_REPEATITION; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TRegexRange : public TreeNode
{
public:
    TRegexRange(Location location)
        : TreeNode(location)
    { }

    virtual ~TRegexRange() = default;

    virtual int
    getType() const
    { return N_REGEX_RANGE; }

    virtual int
    getRule() const
    { return 0; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TRegexRange_Rule1 : public TRegexRange
{
public:
    std::unique_ptr<TStringChar> string_char;

    TRegexRange_Rule1(Location location, TStringChar *string_char)
        : TRegexRange(location), string_char(string_char)
    { }

    virtual int
    getRule() const
    { return 1; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TRegexRange_Rule2 : public TRegexRange
{
public:
    std::unique_ptr<TStringChar> string_char_1;
    std::unique_ptr<TStringChar> string_char_2;

    TRegexRange_Rule2(Location location, TStringChar *string_char_1, TStringChar *string_char_2)
        : TRegexRange(location), string_char_1(string_char_1), string_char_2(string_char_2)
    { }

    virtual int
    getRule() const
    { return 2; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TRegexUnary : public TreeNode
{
public:
    TRegexUnary (Location location)
        : TreeNode(location)
    { }

    virtual ~TRegexUnary() = default;

    virtual int
    getType() const
    { return N_REGEX_UNARY; }

    virtual int
    getRule() const
    { return 0; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TRegexUnary_Rule1 : public TRegexUnary
{
public:
    std::unique_ptr<TStringChar>    string_char;

    TRegexUnary_Rule1(Location location, TStringChar *string_char)
        : TRegexUnary(location), string_char(string_char)
    { }

    virtual ~TRegexUnary_Rule1() = default;

    virtual int
    getRule() const
    { return 1; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TRegexUnary_Rule2 : public TRegexUnary
{
public:
    std::unique_ptr<TRegexContent>  regex_content;

    TRegexUnary_Rule2(Location location, TRegexContent *regex_content)
        : TRegexUnary(location), regex_content(regex_content)
    { }

    virtual ~TRegexUnary_Rule2() = default;

    virtual int
    getRule() const
    { return 2; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TRegexNeg : public TokenNode
{
public:
    TRegexNeg(Location location, std::string literal)
        : TokenNode(location, literal)
    { }

    virtual ~TRegexNeg() = default;

    virtual int
    getType() const
    { return N_REGEX_NEG; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TRegexUnary_Rule3 : public TRegexUnary
{
public:
    std::unique_ptr<TRegexNeg>                  regex_neg_opt;
    std::list<std::unique_ptr<TRegexRange> >    regex_range_plus;

    TRegexUnary_Rule3(Location location, TRegexNeg *regex_neg)
        : TRegexUnary(location), regex_neg_opt(regex_neg)
    { }

    virtual ~TRegexUnary_Rule3() = default;

    virtual int
    getRule() const
    { return 3; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TRegexPart : public TreeNode
{
public:
    std::unique_ptr<TRegexUnary>    regex_unary;
    std::unique_ptr<TRegexRepeatition>  regex_repeatition_opt;

    TRegexPart (Location location, TRegexUnary *regex_unary, TRegexRepeatition *regex_repeatition_opt)
        : TreeNode(location), regex_unary(regex_unary), regex_repeatition_opt(regex_repeatition_opt)
    { }

    virtual ~TRegexPart() = default;

    virtual int
    getType() const
    { return N_REGEX_PART; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TRegexBranch : public TreeNode
{
public:
    std::list<std::unique_ptr<TRegexPart> > regex_part_plus;

    TRegexBranch(Location location)
        : TreeNode(location)
    { }

    virtual ~TRegexBranch() = default;

    virtual int
    getType() const
    { return N_REGEX_BRANCH; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TRegexContent : public TreeNode
{
public:
    struct TRegexContentRep1
    {
        std::unique_ptr<TRegexBranch> regex_branch;
    };

    std::unique_ptr<TRegexBranch>   regex_branch;
    std::list<TRegexContentRep1>    regex_content_rep_1;

    TRegexContent(Location location, TRegexBranch *regex_branch)
        : TreeNode(location), regex_branch(regex_branch)
    { }

    virtual ~TRegexContent() = default;

    virtual int
    getType() const
    { return N_REGEX_CONTENT; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TRegex : public TreeNode
{
public:
    std::unique_ptr<TRegexContent>  regex_content;

    TRegex(Location location, TRegexContent *regex_content)
        : TreeNode(location), regex_content(regex_content)
    { }

    virtual ~TRegex() = default;

    virtual int
    getType() const
    { return N_REGEX; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TStringTransChar : public TokenNode
{
public:
    TStringTransChar(Location location, std::string literal)
        : TokenNode(location, literal)
    { }

    virtual ~TStringTransChar() = default;

    virtual int
    getType() const
    { return N_STRING_TRANS_CHAR; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TStringNonTransChar : public TokenNode
{
public:
    TStringNonTransChar(Location location, std::string literal)
        : TokenNode(location, literal)
    { }

    virtual ~TStringNonTransChar() = default;

    virtual int
    getType() const
    { return N_STRING_NON_TRANS_CHAR; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TStringChar : public TreeNode
{
public:
    std::unique_ptr<TStringNonTransChar>    string_non_trans_char;
    std::unique_ptr<TStringTransChar>       string_trans_char;

    TStringChar(Location location, TStringNonTransChar *string_non_trans_char, TStringTransChar *string_trans_char)
        : TreeNode(location), string_non_trans_char(string_non_trans_char), string_trans_char(string_trans_char)
    { }

    virtual ~TStringChar() = default;

    virtual int
    getType() const
    { return N_STRING_CHAR; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TString : public TreeNode
{
public:
    std::list<std::unique_ptr<TStringChar> >    string_rep_1;

    TString(Location location)
        : TreeNode(location)
    { }

    virtual ~TString() = default;

    virtual int
    getType() const
    { return N_STRING; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TId : public TokenNode
{
public:
    TId(Location location, std::string literal)
        : TokenNode(location, literal)
    { }

    virtual ~TId() = default;

    virtual int
    getType() const
    { return N_ID; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class SentenceRepeatition : public TreeNode
{
public:
    SentenceRepeatition(Location location)
        : TreeNode(location)
    { }

    virtual ~SentenceRepeatition() = default;

    virtual int
    getType() const
    { return N_sentence_repeatition; }

    virtual int
    getRule() const
    { return 0; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class SentenceRepeatition_Rule1 : public SentenceRepeatition
{
public:
    std::unique_ptr<TokenNode> token;

    SentenceRepeatition_Rule1(Location location, TokenNode *token)
        : SentenceRepeatition(location), token(token)
    { }

    virtual ~SentenceRepeatition_Rule1() = default;

    virtual int
    getRule() const
    { return 1; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class SentenceRepeatition_Rule2 : public SentenceRepeatition
{
public:
    std::unique_ptr<TokenNode> token;

    SentenceRepeatition_Rule2(Location location, TokenNode *token)
        : SentenceRepeatition(location), token(token)
    { }

    virtual ~SentenceRepeatition_Rule2() = default;

    virtual int
    getRule() const
    { return 2; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class SentenceRepeatition_Rule3 : public SentenceRepeatition
{
public:
    std::unique_ptr<TokenNode> token;

    SentenceRepeatition_Rule3(Location location, TokenNode *token)
        : SentenceRepeatition(location), token(token)
    { }

    virtual ~SentenceRepeatition_Rule3() = default;

    virtual int
    getRule() const
    { return 3; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class SentenceDecorate : public TreeNode
{
public:
    std::unique_ptr<TokenNode> token;

    SentenceDecorate(Location location, TokenNode *token)
        : TreeNode(location), token(token)
    { }

    virtual ~SentenceDecorate() = default;

    virtual int
    getType() const
    { return N_sentence_decorate; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class SentenceUnary : public TreeNode
{
public:
    SentenceUnary(Location location)
        : TreeNode(location)
    { }

    virtual ~SentenceUnary() = default;

    virtual int
    getType() const
    { return N_sentence_unary; }

    virtual int
    getRule() const
    { return 0; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class SentenceUnary_Rule1 : public SentenceUnary
{
public:
    std::unique_ptr<TId>    id;

    SentenceUnary_Rule1(Location location, TId *id)
        : SentenceUnary(location), id(id)
    { }

    virtual ~SentenceUnary_Rule1() = default;

    virtual int
    getRule() const
    { return 1; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class SentenceUnary_Rule2 : public SentenceUnary
{
public:
    std::unique_ptr<TString>    string;

    SentenceUnary_Rule2(Location location, TString *string)
        : SentenceUnary(location), string(string)
    { }

    virtual ~SentenceUnary_Rule2() = default;

    virtual int
    getRule() const
    { return 2; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class SentenceUnary_Rule3 : public SentenceUnary
{
public:
    std::unique_ptr<SentenceBranch>    sentence_branch;

    SentenceUnary_Rule3(Location location, SentenceBranch *sentence_branch)
        : SentenceUnary(location), sentence_branch(sentence_branch)
    { }

    virtual ~SentenceUnary_Rule3() = default;

    virtual int
    getRule() const
    { return 3; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class SentencePart : public TreeNode
{
public:
    std::unique_ptr<SentenceUnary>          sentence_unary;
    std::unique_ptr<SentenceDecorate>       sentence_decorate_opt;
    std::unique_ptr<SentenceRepeatition>    sentence_repeatition_opt;

    SentencePart(
            Location location,
            SentenceUnary *sentence_unary,
            SentenceDecorate *sentence_decorate_opt,
            SentenceRepeatition *sentence_repeatition_opt
        )
        : TreeNode(location),
        sentence_unary(sentence_unary),
        sentence_decorate_opt(sentence_decorate_opt),
        sentence_repeatition_opt(sentence_repeatition_opt)
    { };

    virtual ~SentencePart() = default;

    virtual int
    getType() const
    { return N_sentence_part; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class SentenceBranch : public TreeNode
{
public:
    std::list<std::unique_ptr<SentencePart> >   sentence_part_rep;

    SentenceBranch(Location location)
        : TreeNode(location)
    { }

    virtual ~SentenceBranch() = default;

    virtual int
    getType() const
    { return N_sentence_branch; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class SentenceDecl : public TreeNode
{
public:
    struct SentenceDeclRep1
    {
        std::unique_ptr<SentenceBranch> sentence_branch;
    };

    std::unique_ptr<SentenceBranch> sentence_branch;
    std::list<SentenceDeclRep1> sentence_decl_rep_1;

    SentenceDecl(Location location, SentenceBranch *sentence_branch)
        : TreeNode(location), sentence_branch(sentence_branch)
    { }

    virtual ~SentenceDecl() = default;

    virtual int
    getType() const
    { return N_sentence_decl; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class SentenceRule : public TreeNode
{
public:
    std::unique_ptr<TId>    id;
    std::unique_ptr<SentenceDecl>   sentence_decl;

    SentenceRule(Location location, TId *id, SentenceDecl *sentence_decl)
        : TreeNode(location), id(id), sentence_decl(sentence_decl)
    { }

    virtual ~SentenceRule() = default;

    virtual int
    getType() const
    { return N_sentence_rule; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};


class TokenRule : public TreeNode
{
public:
    TokenRule(Location location)
        : TreeNode(location)
    { }

    virtual ~TokenRule() = default;

    virtual int
    getType() const
    { return N_token_rule; }

    virtual int
    getRule() const
    { return 0; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TokenRule_Rule1 : public TokenRule
{
public:
    std::unique_ptr<TId>    id;
    std::unique_ptr<TRegex> regex;

    TokenRule_Rule1(Location location, TId *id, TRegex *regex)
        : TokenRule(location), id(id), regex(regex)
    { }

    virtual ~TokenRule_Rule1() = default;

    virtual int
    getRule() const
    { return 1; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TokenRule_Rule2 : public TokenRule
{
public:
    std::unique_ptr<TId>    id;
    std::unique_ptr<SentenceDecl> sentence_decl;

    TokenRule_Rule2(Location location, TId *id, SentenceDecl *sentence_decl)
        : TokenRule(location), id(id), sentence_decl(sentence_decl)
    { }

    virtual ~TokenRule_Rule2() = default;

    virtual int
    getRule() const
    { return 2; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TokenRule_Rule3 : public TokenRule
{
public:
    std::unique_ptr<TId>    id;
    std::unique_ptr<TRegex> regex;

    TokenRule_Rule3(Location location, TId *id, TRegex *regex)
        : TokenRule(location), id(id), regex(regex)
    { }

    virtual ~TokenRule_Rule3() = default;

    virtual int
    getRule() const
    { return 3; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class TokenRule_Rule4 : public TokenRule
{
public:
    std::unique_ptr<TId>    id;
    std::unique_ptr<SentenceDecl> sentence_decl;

    TokenRule_Rule4(Location location, TId *id, SentenceDecl *sentence_decl)
        : TokenRule(location), id(id), sentence_decl(sentence_decl)
    { }

    virtual ~TokenRule_Rule4() = default;

    virtual int
    getRule() const
    { return 4; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class Rule : public TreeNode
{
public:
    Rule(Location location)
        : TreeNode(location)
    { }

    virtual ~Rule() = default;

    virtual int
    getType() const
    { return N_rule; }

    virtual int
    getRule() const
    { return 0; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class Rule_Rule1 : public Rule
{
public:
    std::unique_ptr<TokenRule>  token_rule;

    Rule_Rule1(Location location, TokenRule *token_rule)
        : Rule(location), token_rule(token_rule)
    { }

    virtual ~Rule_Rule1() = default;

    virtual int
    getRule() const
    { return 1; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class Rule_Rule2 : public Rule
{
public:
    std::unique_ptr<SentenceRule>   sentence_rule;

    Rule_Rule2(Location location, SentenceRule *sentence_rule)
        : Rule(location), sentence_rule(sentence_rule)
    { }

    virtual ~Rule_Rule2() = default;

    virtual int
    getRule() const
    { return 2; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class Grammar : public TreeNode
{
public:
    struct GrammarRep1
    {
        std::unique_ptr<Rule> rule;
    };

    std::list<GrammarRep1>  grammar_rep_1;

    Grammar(Location location)
        : TreeNode(location)
    { }

    virtual ~Grammar() = default;

    virtual int
    getType() const
    { return N_grammar; }

    virtual void
    accept(Visitor *v)
    { v->visit(this); }
};

class ParsicalParser
{
public:
    ParsicalParser() { }

    Grammar *parse_file(const std::string &filename);
};

extern Grammar *result;

}

}

#endif
