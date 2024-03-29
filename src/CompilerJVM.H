#ifndef COMPILERJVM_HEADER
#define COMPILERJVM_HEADER

#include <string>
#include <unordered_map>
#include "Compiler.H"

#define MAX_ICONST  5
#define MAX_BIPUSH  127
#define MAX_SIPUSH  32767
#define MAX_ISTORE  3
#define MAX_ILOAD   3

class CompilerJVM : public Compiler
{
public:
  char *compile(Visitable *v);
  char *compile(Visitable *v, std::string fileName);

  void visitProgram(Program *p, bool silent);
  void visitStmt(Stmt *p, bool silent);
  void visitExp(Exp *p, bool silent);
  void visitProg(Prog *p, bool silent);
  void visitSAss(SAss *p, bool silent);
  void visitSExp(SExp *p, bool silent);
  void visitExpAdd(ExpAdd *p, bool silent);
  void visitExpSub(ExpSub *p, bool silent);
  void visitExpMul(ExpMul *p, bool silent);
  void visitExpDiv(ExpDiv *p, bool silent);
  void visitExpLit(ExpLit *p, bool silent);
  void visitExpVar(ExpVar *p, bool silent);
  void visitListStmt(ListStmt *p, bool silent);
  void visitInteger(Integer x, bool silent);
  void visitIdent(Ident x, bool silent);

protected:
  void countStack(int change);
  void visitExpOpHelper(ExpOp *exp_op, bool silent, bool commutative);
  std::string header = ".class public ";//filename/class name should be added here
  std::string header2 = "\n.super java/lang/Object\n\n.method public <init>()V\naload_0\n"
                        "invokespecial java/lang/Object/<init>()V\nreturn\n.end method\n\n"
                        ".method public static main([Ljava/lang/String;)V\n";

  std::string footer = "\nreturn\n.end method\n";

  std::unordered_map <Ident, Integer> storeMap;
  int stackCurrent, stackMax, localCount;

};

#endif /* COMPILERJVM_HEADER */