CC=g++
CCFLAGS=-g -W -Wall -std=c++11

FLEX=flex
FLEX_OPTS=-PInstantLanguage

BISON=bison
BISON_OPTS=-t -pInstantLanguage

OBJS=Absyn.o Lexer.o Parser.o Printer.o Skeleton.o

.PHONY: clean distclean

all: Jwc

clean:
	rm -f *.o TestInstantLanguage InstantLanguage.aux InstantLanguage.log InstantLanguage.pdf InstantLanguage.dvi InstantLanguage.ps InstantLanguage jwc

distclean: clean
	rm -f Absyn.C Absyn.H Test.C Parser.C Parser.H Lexer.C Skeleton.C Skeleton.H Printer.C Printer.H Makefile InstantLanguage.l InstantLanguage.y InstantLanguage.tex 

Jwc: ${OBJS} Jwc.o
	${CC} ${CCFLAGS} ${OBJS} Jwc.o -o jwc

TestInstantLanguage: ${OBJS} Test.o
	@echo "Linking TestInstantLanguage..."
	${CC} ${CCFLAGS} ${OBJS} Test.o -o TestInstantLanguage

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

Printer.o: Printer.C Printer.H Absyn.H
	${CC} ${CCFLAGS} -c Printer.C

Skeleton.o: Skeleton.C Skeleton.H Absyn.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c Skeleton.C

Test.o: Test.C Parser.H Printer.H Absyn.H
	${CC} ${CCFLAGS} -c Test.C
