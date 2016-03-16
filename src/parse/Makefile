all: lex.yy.cpp parse.tab.cpp

lex.yy.cpp: lex.l
	flex -o $@ $<

parse.tab.cpp: parse.y
	bison -d -v -o $@ $<

.PHONY: all clean

clean:
	rm lex.yy.cpp parse.output parse.tab.cpp parse.tab.h
