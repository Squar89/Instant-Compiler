#ifndef COMPILERLLVM_HEADER
#define COMPILERLLVM_HEADER

#include "Compiler.H"

class CompilerLLVM : public Compiler
{
public:
  char *compile(Visitable *v);

  void visitProgram(Program *p);
  void visitStmt(Stmt *p);
  void visitExp(Exp *p);
  void visitProg(Prog *p);
  void visitSAss(SAss *p);
  void visitSExp(SExp *p);
  void visitExpAdd(ExpAdd *p);
  void visitExpSub(ExpSub *p);
  void visitExpMul(ExpMul *p);
  void visitExpDiv(ExpDiv *p);
  void visitExpLit(ExpLit *p);
  void visitExpVar(ExpVar *p);
  void visitListStmt(ListStmt *p);

  void visitInteger(Integer x);
  void visitIdent(Ident x);
};

#endif /* COMPILERLLVM_HEADER */