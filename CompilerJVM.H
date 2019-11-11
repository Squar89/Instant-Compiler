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

protected:
  const std::string header = ".class public Instant\n.super java/lang/Object\n\n.method public <init>()V\naload_0\n"
                             "invokespecial java/lang/Object/<init>()V\nreturn\n.end method\n\n"
                             ".method public static main([Ljava/lang/String;)V\n"
                             /* TODO add counting stack max size */
                             ".limit stack 2000\n.limit locals 20\n";

  const std::string footer = "\ngetstatic java/lang/System/out Ljava/io/PrintStream;\nswap\n"
                             "invokevirtual java/io/PrintStream/println(I)V\n\nreturn\n.end method\n";

  std::unordered_map <Ident, Integer> storeMap;
  int localCount = 0;
};

#endif /* COMPILERJVM_HEADER */