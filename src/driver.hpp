#ifndef _PARSICAL_DRIVER_HPP_
#define _PARSICAL_DRIVER_HPP_

#include <string>
#include <vector>

#include "parsical-parser.hpp"
#include "symbol-info.hpp"
#include "lexer.hpp"

namespace parsical {

class Driver
{
    std::unique_ptr<parser::Grammar> _parser_tree;
    std::unique_ptr<SymbolInfoVisitor> _symbols;
    std::string _output_name;
    std::string _temp_dir;
    std::string _frame_dir;

    std::vector<std::string> _structure_token_list;
public:
    static std::string getOutputNameFromPath(std::string path);
    static std::string capitalUppercase(std::string name);
    static std::string slashConnected(std::string name);

    Driver(int argc, char **argv);

    void generateGlobalLexer();
    void generateStructureLexers();
};

}

#endif
