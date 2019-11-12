CC=g++
CCFLAGS=-g -W -Wall -std=c++11

FLEX=flex
FLEX_OPTS=-PInstantLanguage

BISON=bison
BISON_OPTS=-t -pInstantLanguage

OBJS=Absyn.o Lexer.o Parser.o CompilerJVM.o CompilerLLVM.o

.PHONY: clean distclean

all: Jwc

clean:
	rm -f *.o InstantLanguage.aux InstantLanguage.log InstantLanguage.pdf InstantLanguage.dvi InstantLanguage.ps InstantLanguage jwc

distclean: clean
	rm -f Absyn.C Absyn.H Parser.C Parser.H Lexer.C CompilerJVM.C CompilerJVM.H CompilerLLVM.C CompilerLLVM.H Compiler.H Makefile InstantLanguage.l InstantLanguage.y InstantLanguage.tex 

Jwc: ${OBJS} Jwc.o 
	${CC} ${CCFLAGS} ${OBJS} Jwc.o -o jwc

Absyn.o: Absyn.C Absyn.H
	${CC} ${CCFLAGS} -c Absyn.C

Lexer.C: InstantLanguage.l
	${FLEX} -oLexer.C InstantLanguage.l

Parser.C: InstantLanguage.y
	${BISON} InstantLanguage.y -o Parser.C

Lexer.o: Lexer.C Parser.H
	${CC} ${CCFLAGS} -c Lexer.C 

Parser.o: Parser.C Absyn.H
	${CC} ${CCFLAGS} -c Parser.C

CompilerJVM.o: Compiler.H CompilerJVM.C CompilerJVM.H Absyn.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c CompilerJVM.C

CompilerLLVM.o: Compiler.H CompilerLLVM.C CompilerLLVM.H Absyn.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c CompilerLLVM.C

Jwc.o: Jwc.C Parser.H Absyn.H
	${CC} ${CCFLAGS} -c Jwc.C
