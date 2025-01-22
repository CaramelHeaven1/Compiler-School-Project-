###############################################################################
# purpose: makefile for Project Part 2
# author:  Joe Crumpton / Ed Swan
#
# 'make'        build executable file
# 'make clean'  removes all intermediate (lex.yy.c and *.o) and executable files
# 'make debug'  compile with debugging symbols and run under GDB
#
###############################################################################

# variables used in the following rules
LEX      = flex
CXX      = g++
CC       = gcc
RM       = rm
# -g generate debug information for gdb
# -Wno-c++11-extensions silence the c++11 error warnings
# -std=c++11 assert that we are using c++11
CXXFLAGS = -g -Wno-c++11-extensions -std=c++11
CCFLAGS  = -g
# don't ever remove these files
.PRECIOUS = *.l *.h *.cpp [Mm]akefile

parse: lex.yy.o driver.o parser.o parse_tree_nodes.o
	$(CXX) $(CXXFLAGS) -o tips lex.yy.o driver.o parser.o parse_tree_nodes.o

driver.o: driver.cpp parser.h parse_tree_nodes.h lexer.h
	$(CXX) $(CXXFLAGS) -o driver.o -c driver.cpp

parser.o: parser.cpp parser.h parse_tree_nodes.h lexer.h 
	$(CXX) $(CXXFLAGS) -o parser.o -c parser.cpp

parse_tree_nodes.o: parse_tree_nodes.cpp lexer.h
	$(CXX) $(CXXFLAGS) -o parse_tree_nodes.o -c parse_tree_nodes.cpp

lex.yy.o: lex.yy.c lexer.h
	$(CC) $(CCFLAGS) -o lex.yy.o -c lex.yy.c

lex.yy.c: rules.l lexer.h
	$(LEX) -o lex.yy.c rules.l

clean: 
	$(RM) *.o lex.yy.c tips
#   delete all generated files	

debug: parse
	gdb ./tips
