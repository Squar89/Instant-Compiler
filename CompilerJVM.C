/*** BNFC-Generated Visitor Design Pattern Skeleton. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "Compiler.H"
#include "CompilerJVM.H"

void CompilerJVM::countStack(int change)
{
  this->stackCurrent += change;
  if (this->stackCurrent > this->stackMax) {
    this->stackMax = this->stackCurrent;
  }
}

char *CompilerJVM::compile(Visitable *v)
{
  /* Go through all of the statements appending jvm commands to the buffer */
  visitProg((Prog*) v);

  /* Set .limits for stack size and number of local variables */
  header += ".limit stack ";
  header += std::to_string(this->stackMax);
  header += "\n.limit locals ";
  header += std::to_string(this->localCount > 0 ? this->localCount : 1);
  header += "\n\n";

  return getResult(header.c_str(), footer.c_str());
}

void CompilerJVM::visitProgram(Program *t) {} //abstract class
void CompilerJVM::visitStmt(Stmt *t) {} //abstract class
void CompilerJVM::visitExp(Exp *t) {} //abstract class

void CompilerJVM::visitProg(Prog *prog)
{
  /* reset buffer and all the helper objects/variables */
  bufReset();
  this->storeMap.clear();
  this->localCount = 0;
  this->stackCurrent = 0;
  this->stackMax = 2;

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

  /* Get Ident index and append store command */
  int index = this->storeMap.at(s_ass->ident_);

  countStack(-1);

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
  countStack(1);
  bufAppend("\ngetstatic java/lang/System/out Ljava/io/PrintStream;\n");

  s_exp->exp_->accept(this);

  countStack(-2);
  bufAppend("invokevirtual java/io/PrintStream/println(I)V\n");
}

void CompilerJVM::visitExpAdd(ExpAdd *exp_add)
{
  exp_add->exp_1->accept(this);
  exp_add->exp_2->accept(this);

  countStack(-1);
  bufAppend("iadd\n");
}

void CompilerJVM::visitExpSub(ExpSub *exp_sub)
{
  exp_sub->exp_1->accept(this);
  exp_sub->exp_2->accept(this);

  countStack(-1);
  bufAppend("isub\n");
}

void CompilerJVM::visitExpMul(ExpMul *exp_mul)
{
  exp_mul->exp_1->accept(this);
  exp_mul->exp_2->accept(this);

  countStack(-1);
  bufAppend("imul\n");
}

void CompilerJVM::visitExpDiv(ExpDiv *exp_div)
{
  exp_div->exp_1->accept(this);
  exp_div->exp_2->accept(this);

  countStack(-1);
  bufAppend("idiv\n");
}

void CompilerJVM::visitExpLit(ExpLit *exp_lit)
{
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

  countStack(1);

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
  countStack(1);

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

void CompilerJVM::visitIdent(Ident x) {}
