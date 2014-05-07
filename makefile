CC=cc
CFLAGS=-c -Wall

lexer.c: lexer.lex
	flex -o lexer.c lexer.lex

lexer: lexer.c
	cc -o lexer lexer.c