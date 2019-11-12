/*** BNFC-Generated Visitor Design Pattern Skeleton. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "Compiler.H"
#include "CompilerLLVM.H"

char *CompilerLLVM::compile(Visitable *v)
{
  /* Go through all of the statements appending jvm commands to the buffer */
  visitProg((Prog*) v, false);

  return getResult(header.c_str(), footer.c_str());
}

void CompilerLLVM::visitProgram(Program *t, bool silent) {} //abstract class
void CompilerLLVM::visitStmt(Stmt *t, bool silent) {} //abstract class
void CompilerLLVM::visitExp(Exp *t, bool silent) {} //abstract class

void CompilerLLVM::visitProg(Prog *prog, bool silent)
{
  /* reset buffer and all the helper objects/variables */
  bufReset();
  this->registerCurrent = 1;
  this->variableCurrent = 1;
  this->storeMap.clear();

  prog->liststmt_->accept(this, false);
}

void CompilerLLVM::visitSAss(SAss *s_ass, bool silent)
{
  visitIdent(s_ass->ident_, false);
  s_ass->exp_->accept(this, false);

  /* Check if Ident wasn't already stored */
  if (this->storeMap.count(s_ass->ident_) == 0) {
    this->storeMap.insert(std::make_pair(s_ass->ident_, this->variableCurrent));
    
    /* Append alloca command and increment allocated variables counter */
    bufAppend("%var");
    bufAppend(std::to_string(this->variableCurrent));
    bufAppend(" = alloca i32\n");

    this->variableCurrent++;
  }

  /* Get Ident variable name and append store command */
  int varName = this->storeMap.at(s_ass->ident_);
  bufAppend("store i32 %");
  bufAppend(std::to_string(this->registerCurrent - 1));
  bufAppend(", i32* %var");
  bufAppend(std::to_string(varName));
  bufAppend("\n");
}

void CompilerLLVM::visitSExp(SExp *s_exp, bool silent)
{
  s_exp->exp_->accept(this, false);

  bufAppend("call void @printInt(i32 %");
  bufAppend(std::to_string(this->registerCurrent - 1));
  bufAppend(")\n");
}

void CompilerLLVM::visitExpAdd(ExpAdd *exp_add, bool silent)
{
  int resultRegister1, resultRegister2;

  exp_add->exp_1->accept(this, false);
  resultRegister1 = this->registerCurrent - 1;
  exp_add->exp_2->accept(this, false);
  resultRegister2 = this->registerCurrent - 1;

  bufAppend("%");
  bufAppend(std::to_string(this->registerCurrent));
  bufAppend(" = add i32 %");
  bufAppend(std::to_string(resultRegister1));
  bufAppend(", %");
  bufAppend(std::to_string(resultRegister2));
  bufAppend("\n");

  this->registerCurrent++;
}

void CompilerLLVM::visitExpSub(ExpSub *exp_sub, bool silent)
{
  int resultRegister1, resultRegister2;

  exp_sub->exp_1->accept(this, false);
  resultRegister1 = this->registerCurrent - 1;
  exp_sub->exp_2->accept(this, false);
  resultRegister2 = this->registerCurrent - 1;

  bufAppend("%");
  bufAppend(std::to_string(this->registerCurrent));
  bufAppend(" = sub i32 %");
  bufAppend(std::to_string(resultRegister1));
  bufAppend(", %");
  bufAppend(std::to_string(resultRegister2));
  bufAppend("\n");

  this->registerCurrent++;
}

void CompilerLLVM::visitExpMul(ExpMul *exp_mul, bool silent)
{
  int resultRegister1, resultRegister2;

  exp_mul->exp_1->accept(this, false);
  resultRegister1 = this->registerCurrent - 1;
  exp_mul->exp_2->accept(this, false);
  resultRegister2 = this->registerCurrent - 1;

  bufAppend("%");
  bufAppend(std::to_string(this->registerCurrent));
  bufAppend(" = mul i32 %");
  bufAppend(std::to_string(resultRegister1));
  bufAppend(", %");
  bufAppend(std::to_string(resultRegister2));
  bufAppend("\n");

  this->registerCurrent++;
}

void CompilerLLVM::visitExpDiv(ExpDiv *exp_div, bool silent)
{
  int resultRegister1, resultRegister2;

  exp_div->exp_1->accept(this, false);
  resultRegister1 = this->registerCurrent - 1;
  exp_div->exp_2->accept(this, false);
  resultRegister2 = this->registerCurrent - 1;

  bufAppend("%");
  bufAppend(std::to_string(this->registerCurrent));
  bufAppend(" = div i32 %");
  bufAppend(std::to_string(resultRegister1));
  bufAppend(", %");
  bufAppend(std::to_string(resultRegister2));
  bufAppend("\n");

  this->registerCurrent++;
}

void CompilerLLVM::visitExpLit(ExpLit *exp_lit, bool silent)
{
  /* Code For ExpLit Goes Here */

  visitInteger(exp_lit->integer_, false);

}

void CompilerLLVM::visitExpVar(ExpVar *exp_var, bool silent)
{
  visitIdent(exp_var->ident_, false);

  /* Check if variable with such ident exists */
  if (this->storeMap.count(exp_var->ident_) == 0) {
    throw "Variable " + exp_var->ident_ + " used but not assigned";
  }

  /* Get ident variable name and append load command */
  int varName = this->storeMap.at(exp_var->ident_);

  bufAppend("%");
  bufAppend(std::to_string(this->registerCurrent));
  bufAppend(" = load i32, i32* %var");
  bufAppend(std::to_string(varName));
  bufAppend("\n");

  this->registerCurrent++;
}

void CompilerLLVM::visitListStmt(ListStmt *list_stmt, bool silent)
{
  for (ListStmt::iterator i = list_stmt->begin() ; i != list_stmt->end() ; ++i)
  {
    (*i)->accept(this, false);
  }
}

void CompilerLLVM::visitInteger(Integer x, bool silent)
{
  bufAppend("%");
  bufAppend(std::to_string(this->registerCurrent));
  bufAppend(" = add i32 0, ");
  bufAppend(std::to_string(x));
  bufAppend("\n");

  this->registerCurrent++;
}

void CompilerLLVM::visitIdent(Ident x, bool silent)
{
  /* Code for Ident Goes Here */
}



