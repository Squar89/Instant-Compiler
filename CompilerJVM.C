/*** BNFC-Generated Visitor Design Pattern Skeleton. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "Compiler.H"
#include "CompilerJVM.H"

char *CompilerJVM::compile(Visitable *v) {
  bufReset();
  bufAppend(header);

  visitProg((Prog*) v);

  bufAppend(footer);

  return buf_;
}

void CompilerJVM::visitProgram(Program *t) {} //abstract class
void CompilerJVM::visitStmt(Stmt *t) {} //abstract class
void CompilerJVM::visitExp(Exp *t) {} //abstract class

void CompilerJVM::visitProg(Prog *prog)
{
  /* Code For Prog Goes Here */

  prog->liststmt_->accept(this);

}

void CompilerJVM::visitSAss(SAss *s_ass)
{
  /* Code For SAss Goes Here */

  visitIdent(s_ass->ident_);
  s_ass->exp_->accept(this);

}

void CompilerJVM::visitSExp(SExp *s_exp)
{
  /* Code For SExp Goes Here */

  s_exp->exp_->accept(this);

}

void CompilerJVM::visitExpAdd(ExpAdd *exp_add)
{
  exp_add->exp_1->accept(this);
  exp_add->exp_2->accept(this);

  bufAppend("iadd\n");
}

void CompilerJVM::visitExpSub(ExpSub *exp_sub)
{
  exp_sub->exp_1->accept(this);
  exp_sub->exp_2->accept(this);

  bufAppend("isub\n");
}

void CompilerJVM::visitExpMul(ExpMul *exp_mul)
{
  exp_mul->exp_1->accept(this);
  exp_mul->exp_2->accept(this);

  bufAppend("imul\n");
}

void CompilerJVM::visitExpDiv(ExpDiv *exp_div)
{
  exp_div->exp_1->accept(this);
  exp_div->exp_2->accept(this);

  bufAppend("idiv\n");
}

void CompilerJVM::visitExpLit(ExpLit *exp_lit)
{
  /* Code For ExpLit Goes Here */

  visitInteger(exp_lit->integer_);

}

void CompilerJVM::visitExpVar(ExpVar *exp_var)
{
  /* Code For ExpVar Goes Here */

  visitIdent(exp_var->ident_);

}

void CompilerJVM::visitListStmt(ListStmt *list_stmt)
{
  for (ListStmt::iterator i = list_stmt->begin() ; i != list_stmt->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void CompilerJVM::visitInteger(Integer x)
{
  if (x < 0) {
    throw "Negative integer explicitly used in expression";
  }
  else if (x < 6) {
    bufAppend("iconst_");
  }
  else if (x < 128) {
    bufAppend("bipush ");
  }
  else if (x < 32768) {
    bufAppend("sipush ");
  }
  else {
    bufAppend("ldc ");
  }
  
  bufAppend(std::to_string(x));
  bufAppend("\n");
}

void CompilerJVM::visitIdent(Ident x)
{
  /* Code for Ident Goes Here */
}



