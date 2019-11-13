#ifndef COMPILERLLVM_HEADER
#define COMPILERLLVM_HEADER

#include <string>
#include <unordered_map>
#include "Compiler.H"

class CompilerLLVM : public Compiler
{
public:
  char *compile(Visitable *v);

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
  void visitExpOpHelper(ExpOp *exp_op, bool silent, std::string opCommand);
  std::string header = "declare void @printInt(i32)\ndefine i32 @main() {\n";
  std::string footer = "\nret i32 0\n}\n";
  int registerCurrent, variableCurrent;
  std::unordered_map <Ident, Integer> storeMap;
  bool lastResultWasLiteral;//this tells us if we should look for a value in last register or in int below
  int lastLiteralValue;
};

#endif /* COMPILERLLVM_HEADER */