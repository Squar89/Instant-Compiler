/*** BNFC-Generated Visitor Design Pattern Skeleton. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "Compiler.H"
#include "CompilerJVM.H"

char *CompilerJVM::compile(Visitable *v) {
  bufReset();
  this->storeMap.clear();
  this->localCount = 0;
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
  visitIdent(s_ass->ident_);
  s_ass->exp_->accept(this);

  /* Check if Ident wasn't already stored and increment local variables counter if it wasn't */
  if (this->storeMap.count(s_ass->ident_) == 0) {
    this->storeMap.insert(std::make_pair(s_ass->ident_, this->localCount));
    this->localCount++;
  }

  /* Get ident index and append store command */
  int index = this->storeMap.at(s_ass->ident_);

  bufAppend("istore");
  if (index <= MAX_ISTORE) {
    bufAppend("_");
  }
  else {
    bufAppend(" ");
  }
  bufAppend(std::to_string(index));
  bufAppend("\n");
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
  visitIdent(exp_var->ident_);

  /* Check if variable with such ident exists */
  if (this->storeMap.count(exp_var->ident_) == 0) {
    throw "Variable " + exp_var->ident_ + " used but not assigned";
  }

  /* Get ident index and append load command */
  int index = this->storeMap.at(exp_var->ident_);

  bufAppend("iload");
  if (index <= MAX_ILOAD) {
    bufAppend("_");
  }
  else {
    bufAppend(" ");
  }
  bufAppend(std::to_string(index));
  bufAppend("\n");

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
  else if (x <= MAX_ICONST) {
    bufAppend("iconst_");
  }
  else if (x <= MAX_BIPUSH) {
    bufAppend("bipush ");
  }
  else if (x <= MAX_SIPUSH) {
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



