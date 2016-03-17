add_executable(parsical main.cpp parsical-parser.cpp parse/lex.yy.cpp parse/parse.tab.cpp print-visitor.cpp token-info.cpp fa.cpp)

add_custom_command(
    OUTPUT ${PROJECT_SOURCE_DIR}/src/parse/lex.yy.cpp
    COMMAND flex -o ${PROJECT_SOURCE_DIR}/src/parse/lex.yy.cpp ${PROJECT_SOURCE_DIR}/src/parse/lex.l
    DEPENDS ${PROJECT_SOURCE_DIR}/src/parse/lex.l
)

add_custom_command(
    OUTPUT ${PROJECT_SOURCE_DIR}/src/parse/parse.tab.cpp
    COMMAND bison -d -v -o ${PROJECT_SOURCE_DIR}/src/parse/parse.tab.cpp ${PROJECT_SOURCE_DIR}/src/parse/parse.y
    DEPENDS ${PROJECT_SOURCE_DIR}/src/parse/parse.y
)
