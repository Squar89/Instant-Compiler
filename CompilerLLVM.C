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
  this->lastResultWasLiteral = false;

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

  /* Get Ident variable name */
  int varName = this->storeMap.at(s_ass->ident_);

  /* Append store command with expression result */
  /* Expression result can be either literal value or complex expression with result stored in previous register */
  bufAppend("store i32 ");
  if (this->lastResultWasLiteral) {
    bufAppend(std::to_string(this->lastLiteralValue));
    this->lastResultWasLiteral = false;
  }
  else {
    bufAppend("%");
    bufAppend(std::to_string(this->registerCurrent - 1));
  }
  bufAppend(", i32* %var");
  bufAppend(std::to_string(varName));
  bufAppend("\n");
}

void CompilerLLVM::visitSExp(SExp *s_exp, bool silent)
{
  s_exp->exp_->accept(this, false);

  /* Expression result can be either literal value or complex expression with result stored in previous register */
  bufAppend("call void @printInt(i32 ");
  if (this->lastResultWasLiteral) {
    bufAppend(std::to_string(this->lastLiteralValue));
    this->lastResultWasLiteral = false;
  }
  else {
    bufAppend("%");
    bufAppend(std::to_string(this->registerCurrent - 1));
  }
  bufAppend(")\n");
}

/* This function is abstraction on all the arithmetical operations visits */
void CompilerLLVM::visitExpOpHelper(ExpOp *exp_op, bool silent, std::string opCommand)
{
  int result1, result2;
  bool resultIsReg1, resultIsReg2;//if set to true, result1/result2 stores register and not integer

  exp_op->exp_1->accept(this, false);
  /* Check if first expression is simply an integer, if so set result1 to its value */
  if (this->lastResultWasLiteral) {
    resultIsReg1 = false;
    result1 = this->lastLiteralValue;
  }
  /* Otherwise set result1 to register containing expression result */
  else {
    resultIsReg1 = true;
    result1 = this->registerCurrent - 1;
  }
  this->lastResultWasLiteral = false;

  /* Repeat the same for second expression */
  exp_op->exp_2->accept(this, false);
  if (this->lastResultWasLiteral) {
    resultIsReg2 = false;
    result2 = this->lastLiteralValue;
  }
  else {
    resultIsReg2 = true;
    result2 = this->registerCurrent - 1;
  }
  this->lastResultWasLiteral = false;

  bufAppend("%");
  bufAppend(std::to_string(this->registerCurrent));
  bufAppend(" = ");
  bufAppend(opCommand);
  bufAppend(" i32 ");
  if (resultIsReg1) {
    bufAppend("%");
  }
  bufAppend(std::to_string(result1));
  bufAppend(", ");
  if (resultIsReg2) {
    bufAppend("%");
  }
  bufAppend(std::to_string(result2));
  bufAppend("\n");

  this->registerCurrent++;
}

void CompilerLLVM::visitExpAdd(ExpAdd *exp_add, bool silent)
{
  visitExpOpHelper(exp_add, silent, "add");
}

void CompilerLLVM::visitExpSub(ExpSub *exp_sub, bool silent)
{
  visitExpOpHelper(exp_sub, silent, "sub");
}

void CompilerLLVM::visitExpMul(ExpMul *exp_mul, bool silent)
{
  visitExpOpHelper(exp_mul, silent, "mul");
}

void CompilerLLVM::visitExpDiv(ExpDiv *exp_div, bool silent)
{
  visitExpOpHelper(exp_div, silent, "div");
}

void CompilerLLVM::visitExpLit(ExpLit *exp_lit, bool silent)
{
  this->lastResultWasLiteral = true;
  this->lastLiteralValue = exp_lit->integer_;
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

void CompilerLLVM::visitInteger(Integer x, bool silent) {}

void CompilerLLVM::visitIdent(Ident x, bool silent) {}



