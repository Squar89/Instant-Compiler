/*** BNFC-Generated Visitor Design Pattern Skeleton. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "Compiler.H"
#include "CompilerLLVM.H"

char *CompilerLLVM::compile(Visitable *v) {
  //TODO
  return buf_;
}

void CompilerLLVM::visitProgram(Program *t, bool silent) {} //abstract class
void CompilerLLVM::visitStmt(Stmt *t, bool silent) {} //abstract class
void CompilerLLVM::visitExp(Exp *t, bool silent) {} //abstract class

void CompilerLLVM::visitProg(Prog *prog, bool silent)
{
  /* Code For Prog Goes Here */
  printf("Hello World from Prog!\n");

  prog->liststmt_->accept(this, false);

}

void CompilerLLVM::visitSAss(SAss *s_ass, bool silent)
{
  /* Code For SAss Goes Here */
  printf("Hello World from SAss!\n");

  visitIdent(s_ass->ident_, false);
  s_ass->exp_->accept(this, false);

}

void CompilerLLVM::visitSExp(SExp *s_exp, bool silent)
{
  /* Code For SExp Goes Here */
  printf("Hello World from SExp!\n");

  s_exp->exp_->accept(this, false);

}

void CompilerLLVM::visitExpAdd(ExpAdd *exp_add, bool silent)
{
  /* Code For ExpAdd Goes Here */
  printf("Hello World from ExpAdd!\n");

  exp_add->exp_1->accept(this, false);
  exp_add->exp_2->accept(this, false);

}

void CompilerLLVM::visitExpSub(ExpSub *exp_sub, bool silent)
{
  /* Code For ExpSub Goes Here */
  printf("Hello World from ExpSub!\n");

  exp_sub->exp_1->accept(this, false);
  exp_sub->exp_2->accept(this, false);

}

void CompilerLLVM::visitExpMul(ExpMul *exp_mul, bool silent)
{
  /* Code For ExpMul Goes Here */
  printf("Hello World from ExpMul!\n");

  exp_mul->exp_1->accept(this, false);
  exp_mul->exp_2->accept(this, false);

}

void CompilerLLVM::visitExpDiv(ExpDiv *exp_div, bool silent)
{
  /* Code For ExpDiv Goes Here */
  printf("Hello World from ExpDiv!\n");

  exp_div->exp_1->accept(this, false);
  exp_div->exp_2->accept(this, false);

}

void CompilerLLVM::visitExpLit(ExpLit *exp_lit, bool silent)
{
  /* Code For ExpLit Goes Here */
  printf("Hello World from ExpLit!\n");

  visitInteger(exp_lit->integer_, false);

}

void CompilerLLVM::visitExpVar(ExpVar *exp_var, bool silent)
{
  /* Code For ExpVar Goes Here */
  printf("Hello World from ExpVar!\n");

  visitIdent(exp_var->ident_, false);

}

void CompilerLLVM::visitListStmt(ListStmt *list_stmt, bool silent)
{
  printf("Hello World from ListStmt!\n");
  for (ListStmt::iterator i = list_stmt->begin() ; i != list_stmt->end() ; ++i)
  {
    (*i)->accept(this, false);
  }
}

void CompilerLLVM::visitInteger(Integer x, bool silent)
{
  /* Code for Integer Goes Here */
  printf("Hello World from Integer!\n");
}

void CompilerLLVM::visitIdent(Ident x, bool silent)
{
  /* Code for Ident Goes Here */
  printf("Hello World from Ident!\n");
}



