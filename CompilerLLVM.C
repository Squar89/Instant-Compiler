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

void CompilerLLVM::visitProgram(Program *t) {} //abstract class
void CompilerLLVM::visitStmt(Stmt *t) {} //abstract class
void CompilerLLVM::visitExp(Exp *t) {} //abstract class

void CompilerLLVM::visitProg(Prog *prog)
{
  /* Code For Prog Goes Here */
  printf("Hello World from Prog!\n");

  prog->liststmt_->accept(this);

}

void CompilerLLVM::visitSAss(SAss *s_ass)
{
  /* Code For SAss Goes Here */
  printf("Hello World from SAss!\n");

  visitIdent(s_ass->ident_);
  s_ass->exp_->accept(this);

}

void CompilerLLVM::visitSExp(SExp *s_exp)
{
  /* Code For SExp Goes Here */
  printf("Hello World from SExp!\n");

  s_exp->exp_->accept(this);

}

void CompilerLLVM::visitExpAdd(ExpAdd *exp_add)
{
  /* Code For ExpAdd Goes Here */
  printf("Hello World from ExpAdd!\n");

  exp_add->exp_1->accept(this);
  exp_add->exp_2->accept(this);

}

void CompilerLLVM::visitExpSub(ExpSub *exp_sub)
{
  /* Code For ExpSub Goes Here */
  printf("Hello World from ExpSub!\n");

  exp_sub->exp_1->accept(this);
  exp_sub->exp_2->accept(this);

}

void CompilerLLVM::visitExpMul(ExpMul *exp_mul)
{
  /* Code For ExpMul Goes Here */
  printf("Hello World from ExpMul!\n");

  exp_mul->exp_1->accept(this);
  exp_mul->exp_2->accept(this);

}

void CompilerLLVM::visitExpDiv(ExpDiv *exp_div)
{
  /* Code For ExpDiv Goes Here */
  printf("Hello World from ExpDiv!\n");

  exp_div->exp_1->accept(this);
  exp_div->exp_2->accept(this);

}

void CompilerLLVM::visitExpLit(ExpLit *exp_lit)
{
  /* Code For ExpLit Goes Here */
  printf("Hello World from ExpLit!\n");

  visitInteger(exp_lit->integer_);

}

void CompilerLLVM::visitExpVar(ExpVar *exp_var)
{
  /* Code For ExpVar Goes Here */
  printf("Hello World from ExpVar!\n");

  visitIdent(exp_var->ident_);

}

void CompilerLLVM::visitListStmt(ListStmt *list_stmt)
{
  printf("Hello World from ListStmt!\n");
  for (ListStmt::iterator i = list_stmt->begin() ; i != list_stmt->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void CompilerLLVM::visitInteger(Integer x)
{
  /* Code for Integer Goes Here */
  printf("Hello World from Integer!\n");
}

void CompilerLLVM::visitIdent(Ident x)
{
  /* Code for Ident Goes Here */
  printf("Hello World from Ident!\n");
}



