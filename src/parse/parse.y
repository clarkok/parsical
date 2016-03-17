%{
#include <iostream>
#include "../parsical-parser.hpp"
#include "parse.tab.hpp"

#include "parse_regex.hpp"

using namespace parsical::parser;

extern int yylineno;
int yylex();
int yyerror(const char *);

#define _current_location   \
    Location("", yylineno, 0)
%}

%token ID STRING REGEX
%debug

%union {
    parsical::parser::TreeNode *node;
    const char *literal;
}

%type<node>     grammar rule token_rule sentence_rule sentence_decl sentence_branch sentence_part
%type<node>     sentence_unary sentence_decorate sentence_repeatition id string regex
%type<literal>  ID STRING REGEX

%%
grammar:
      grammar rule
        {
            auto *grammar = dynamic_cast<Grammar*>($1);
            Grammar::GrammarRep1 rep;
            rep.rule.reset(dynamic_cast<Rule*>($2));
            grammar->grammar_rep_1.emplace_back(std::move(rep));
            $$ = grammar;

            result = dynamic_cast<Grammar*>($$);
        }
    | rule
        {
            auto *grammar = new Grammar(_current_location);
            Grammar::GrammarRep1 rep;
            rep.rule.reset(dynamic_cast<Rule*>($1));
            grammar->grammar_rep_1.emplace_back(std::move(rep));
            $$ = grammar;

            result = dynamic_cast<Grammar*>($$);
        }
    ;

rule:
      token_rule ';'
        {
            $$ = new Rule_Rule1(_current_location, dynamic_cast<TokenRule*>($1));
        }
    | sentence_rule ';'
        {
            $$ = new Rule_Rule2(_current_location, dynamic_cast<SentenceRule*>($1));
        }
    ;

token_rule:
      id '=' regex
        {
            $$ = new TokenRule_Rule1(_current_location, dynamic_cast<TId*>($1), dynamic_cast<TRegex*>($3));
        }
    | id '=' sentence_decl
        {
            $$ = new TokenRule_Rule2(_current_location, dynamic_cast<TId*>($1), dynamic_cast<SentenceDecl*>($3));
        }
    | '~' id '=' regex
        {
            $$ = new TokenRule_Rule3(_current_location, dynamic_cast<TId*>($2), dynamic_cast<TRegex*>($4));
        }
    ;

sentence_rule:
      id ':' sentence_decl
        {
            $$ = new SentenceRule(_current_location, dynamic_cast<TId*>($1), dynamic_cast<SentenceDecl*>($3));
        }
    ;

sentence_decl:
      sentence_decl '|' sentence_branch
        {
            auto *sentence_decl = dynamic_cast<SentenceDecl*>($1);
            SentenceDecl::SentenceDeclRep1 rep;
            rep.sentence_branch.reset(dynamic_cast<SentenceBranch*>($3));
            sentence_decl->sentence_decl_rep_1.emplace_back(std::move(rep));
            $$ = sentence_decl;
        }
    | sentence_branch
        {
            $$ = new SentenceDecl(_current_location, dynamic_cast<SentenceBranch*>($1));
        }
    ;

sentence_branch:
      sentence_branch sentence_part
        {
            auto *sentence_branch = dynamic_cast<SentenceBranch*>($1);
            sentence_branch->sentence_part_rep.emplace_back(dynamic_cast<SentencePart*>($2));
            $$ = sentence_branch;
        }
    | sentence_part
        {
            auto *sentence_branch = new SentenceBranch(_current_location);
            sentence_branch->sentence_part_rep.emplace_back(dynamic_cast<SentencePart*>($1));
            $$ = sentence_branch;
        }
    ;

sentence_part:
      sentence_unary sentence_decorate sentence_repeatition
        {
            $$ = new SentencePart(
                _current_location,
                dynamic_cast<SentenceUnary*>($1),
                dynamic_cast<SentenceDecorate*>($2),
                dynamic_cast<SentenceRepeatition*>($3)
            );
        }
    | sentence_unary                   sentence_repeatition
        {
            $$ = new SentencePart(
                _current_location,
                dynamic_cast<SentenceUnary*>($1),
                nullptr,
                dynamic_cast<SentenceRepeatition*>($2)
            );
        }
    | sentence_unary sentence_decorate
        {
            $$ = new SentencePart(
                _current_location,
                dynamic_cast<SentenceUnary*>($1),
                dynamic_cast<SentenceDecorate*>($2),
                nullptr
            );
        }
    | sentence_unary
        {
            $$ = new SentencePart(
                _current_location,
                dynamic_cast<SentenceUnary*>($1),
                nullptr,
                nullptr
            );
        }
    ;

sentence_unary:
      id
        {
            $$ = new SentenceUnary_Rule1(_current_location, dynamic_cast<TId*>($1));
        }
    | string
        {
            $$ = new SentenceUnary_Rule2(_current_location, dynamic_cast<TString*>($1));
        }
    | '(' sentence_branch ')'
        {
            $$ = new SentenceUnary_Rule3(_current_location, dynamic_cast<SentenceBranch*>($2));
        }
    ;

sentence_decorate:
      '!'
        {
            $$ = new SentenceDecorate(
                    _current_location,
                    new TokenNode(
                        _current_location,
                        "!"
                    )
                );
        }
    ;

sentence_repeatition:
      '*'
        {
            $$ = new SentenceRepeatition_Rule1(
                    _current_location,
                    new TokenNode(
                        _current_location,
                        "*"
                    )
                );
        }
    | '+'
        {
            $$ = new SentenceRepeatition_Rule2(
                    _current_location,
                    new TokenNode(
                        _current_location,
                        "+"
                    )
                );
        }
    | '?'
        {
            $$ = new SentenceRepeatition_Rule3(
                    _current_location,
                    new TokenNode(
                        _current_location,
                        "?"
                    )
                );
        }
    ;

id:
      ID
        {
            $$ = new TId(
                _current_location,
                $1
            );
        }
    ;

string:
      STRING
        {
            $$ = parse_string(_current_location, $1);
        }
    ;

regex:
      REGEX
        {
            $$ = parse_regex(_current_location, $1);
        }
    ;
%%
int yyerror(const char *msg)
{
    std::cerr << msg << std::endl;
    std::cerr << yylineno << std::endl;
    exit(-1);
}
