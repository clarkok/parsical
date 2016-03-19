#include <cstdlib>
#include <fstream>
#include <iostream>

#include "report.hpp"
#include "driver.hpp"
#include "normalize.hpp"

#include "lexer.hpp"

using namespace parsical;

Driver::Driver(int argc, char **argv)
{
    if (!argc) {
        Report::getReport().error("No input file");
    }

    parser::ParsicalParser pp;
    _parser_tree.reset(pp.parse_file(*argv));

    NormalizeVisitor(_parser_tree.get());

    _symbols.reset(new SymbolInfoVisitor());
    _parser_tree->accept(_symbols.get());

    {
        std::ofstream fsymbols("./output.symbols");

        for (auto iter = _symbols->fragmentBegin(); iter != _symbols->fragmentEnd(); ++iter) {
            fsymbols << iter->first << std::endl;
        }

        fsymbols << std::endl;

        for (auto iter = _symbols->symbolBegin(); iter != _symbols->symbolEnd(); ++iter) {
            fsymbols << iter->first << std::endl;
        }
    }

    _output_name = getOutputNameFromPath(*argv);

    auto *temp = std::getenv("TEMPDIR");
    _temp_dir = temp ? temp : "/tmp/";
    if (_temp_dir[_temp_dir.length() - 1] != '/') {
        _temp_dir += '/';
    }

    // for debug
    _frame_dir = "/home/c/c-stack/parsical/frame/";

    // get structure token list
    for (auto iter = _symbols->symbolBegin(); iter != _symbols->symbolEnd(); ++iter) {
        if (_symbols->hasFragment(iter->first)) {
            _structure_token_list.push_back(iter->first);
        }
    }

    std::system(("cp " + _frame_dir + "* " + _temp_dir).c_str());
}

void
Driver::generateGlobalLexer()
{
    std::string lex_name(_temp_dir + _output_name + ".global.lex.cpp");

    Lexer lexer(_parser_tree.get());

    {
        std::ofstream flex_name(_temp_dir + "output.lexname");
        flex_name << "Lexer";
    }

    {
        std::ofstream ftable(_temp_dir + "output.lextable");
        lexer.outputFATables(ftable);
    }

    std::cout << ("cpp -o " + lex_name + " " + _temp_dir + "lexer.frame").c_str() << std::endl;
    std::system(("cpp -o " + lex_name + " " + _temp_dir + "lexer.frame").c_str());
}

void
Driver::generateStructureLexers()
{
    for (auto &token : _structure_token_list) {
        Lexer lexer(new FragmentTokenInfoVisitor(_symbols.get(), token));
        std::string lex_name(_temp_dir + slashConnected(token) + ".lex.cpp");

        {
            std::ofstream flex_name(_temp_dir + "output.lexname");
            flex_name << capitalUppercase(token) << "Lexer";
        }

        {
            std::ofstream ftable(_temp_dir + "output.lextable");
            lexer.outputFATables(ftable);
        }

        std::cout << ("cpp -o " + lex_name + " " + _temp_dir + "lexer.frame").c_str() << std::endl;
        std::system(("cpp -o " + lex_name + " " + _temp_dir + "lexer.frame").c_str());
        std::system(("rm " + _temp_dir + "output.lexname " + _temp_dir + "output.lextable").c_str());
    }
}

std::string
Driver::getOutputNameFromPath(std::string path)
{
    int pos;

    while ((pos = path.find('/')) != -1) {
        path = path.substr(pos + 1);
    }

    std::cout << path << std::endl;

    pos = path.find('.');

    return path.substr(0, pos);
}

std::string
Driver::capitalUppercase(std::string name)
{
    // TODO 
    return name;
}

std::string
Driver::slashConnected(std::string name)
{
    // TODO
    return name;
}
