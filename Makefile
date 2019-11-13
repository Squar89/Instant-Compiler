CC=g++
CCFLAGS=-g -std=c++11

FLEX=flex
FLEX_OPTS=-PInstantLanguage

BISON=bison
BISON_OPTS=-t -pInstantLanguage

OBJS=Absyn.o Lexer.o Parser.o CompilerJVM.o CompilerLLVM.o

.PHONY: clean

all: InscJVM InscLLVM

clean:
	rm -f *.o src/*.o insc_jvm insc_llvm tests/*.class tests/*.j tests/*.bc tests/*.ll *.bc lib/*.bc

InscJVM: ${OBJS} src/InscJVM.o
	${CC} ${CCFLAGS} ${OBJS} src/InscJVM.o -o insc_jvm

InscLLVM: ${OBJS} src/InscLLVM.o
	${CC} ${CCFLAGS} ${OBJS} src/InscLLVM.o -o insc_llvm

Absyn.o: src/Absyn.C src/Absyn.H
	${CC} ${CCFLAGS} -c src/Absyn.C

Lexer.C: src/InstantLanguage.l
	${FLEX} -o src/Lexer.C src/InstantLanguage.l

Parser.C: src/InstantLanguage.y
	${BISON} src/InstantLanguage.y -o src/Parser.C

Lexer.o: src/Lexer.C src/Parser.H
	${CC} ${CCFLAGS} -c src/Lexer.C 

Parser.o: src/Parser.C src/Absyn.H
	${CC} ${CCFLAGS} -c src/Parser.C

CompilerJVM.o: src/Compiler.H src/CompilerJVM.C src/CompilerJVM.H src/Absyn.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c src/CompilerJVM.C

CompilerLLVM.o: src/Compiler.H src/CompilerLLVM.C src/CompilerLLVM.H src/Absyn.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c src/CompilerLLVM.C

InscJVM.o: src/InscJVM.C src/Parser.H src/Absyn.H
	${CC} ${CCFLAGS} -c src/InscJVM.C

InscLLVM.o: src/InscLLVM.C src/Parser.H src/Absyn.H
	${CC} ${CCFLAGS} -c InscLLVM.C