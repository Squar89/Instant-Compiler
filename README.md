# MRJP1
A compiler for the language Instant - description: http://www.mimuw.edu.pl/~ben/Zajecia/Mrj2019/instant-en.html

# Instant grammar
Prog. Program ::= [Stmt] ;  
SAss. Stmt ::= Ident "=" Exp;  
SExp. Stmt ::= Exp ;  
separator Stmt ";" ;  

ExpAdd.            Exp1   ::= Exp2 "+"  Exp1 ;  
ExpSub.            Exp2   ::= Exp2 "-"  Exp3 ;  
ExpMul.            Exp3   ::= Exp3 "*"  Exp4 ;  
ExpDiv.            Exp3   ::= Exp3 "/"  Exp4 ;  
ExpLit.            Exp4   ::= Integer ;  
ExpVar.            Exp4   ::= Ident ;  
coercions Exp 4;  

It can also be found in file InstantLanguage.cf inside /src directory

# Optimizations used in compilers
## JVM Compiler
### Stack size optimization by expressions execution order
This optimization is pretty straight forward. By knowing which subexpression from the two in add (or any other arithmetical operation) will require more stack space we can execute it first and minimize stack space used by add operation. So the stack space used will be a (minimum + 1) from the two subexpression stack usages. Because we are changing order in which those expressions are executed and therefore placed on stack, we need to add swap operation for non-commutative operations.
### Limiting stack space and number of local variables
Throughout the compilation the number of local variables and stack usage are counted and later on limited by the .limit jasmin command.
### Remembering stack usage for each expression
Stack usage for each expression is only counted once and saved to stackUsage variable inside Exp object. Thanks to this, we are maintaining linear complexity while still optimizing stack size by exection order. This is achieved by using "silent" visitor, the one that doesn't append anything to buffer, counts stack usage by expression and visits each expression only once.

## LLVM Compiler
The LLVM compiler is pretty straightforward. The only thing that might not seem obvious is remembering if the expression was a literal or not, since we are not forcing single literals onto registers (using %1 = add i32 0, x).

# Repository layout
## lib
Third-party code is found here. More on those in the section "Used libraries/third-party code" in this README
## test
Some basic tests I used while debugging the code. Nothing to fancy, just enough to see if the compiler works properly. Stress tests were also used during testing phase to make sure the compilers time complexity is linear.
## src
All of the source files are found here. I hope the file structure is pretty clear so I won't get into details about each one of them here.

# Used libraries/third-party code
## BNFC
Some of the source files such as Absyn.*, Lexer.*, Parser.* were generated using BNFC (available here http://bnfc.digitalgrammars.com). This neat BNF converter generated the backbone of this compiler (based on grammar) allowing me to kickstart the project and get right into the coding without worrying about parser/lexer.
## Jasmin
Available in the lib directory as jar file allowed me to transform generated jvm code into .class files. More info on this tool is available here: http://jasmin.sourceforge.net.
## Runtime.ll
Found in lib directory this file supplied by the lecturer has various print functions in llvm. My compiler uses PrintInt function from this file to print results of each Expression.
