#ifndef COMPILERJVM_HEADER
#define COMPILERJVM_HEADER

#include "Compiler.H"
#include <string>

class CompilerJVM : public Compiler
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
  void visitChar(Char x);
  void visitDouble(Double x);
  void visitString(String x);
  void visitIdent(Ident x);

protected:
  const std::string header = ".class public Instant\n.super java/lang/Object\n\n.method public <init>()V\naload_0\n"
                             "invokespecial java/lang/Object/<init>()V\nreturn\n.end method\n\n"
                             ".method public static main([Ljava/lang/String;)V\n"
                             /* TODO add counting stack max size */
                             ".limit stack 2000\n";
};

#endif /* COMPILERJVM_HEADER */