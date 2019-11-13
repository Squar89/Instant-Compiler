/*** BNFC-Generated Visitor Design Pattern Skeleton. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include <iostream>
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
  compile(v, "Instant");
}

char *CompilerJVM::compile(Visitable *v, std::string fileName)
{
  /* Append filename to the header */
  header += fileName;
  header += header2;

  /* Go through all of the statements appending jvm commands to the buffer */
  visitProg((Prog*) v, false);

  /* Set .limits for stack size and number of local variables */
  header += ".limit stack ";
  header += std::to_string(this->stackMax);
  header += "\n.limit locals ";
  header += std::to_string(this->localCount > 0 ? this->localCount : 1);
  header += "\n\n";

  return getResult(header.c_str(), footer.c_str());
}

void CompilerJVM::visitProgram(Program *t, bool silent) {} //abstract class
void CompilerJVM::visitStmt(Stmt *t, bool silent) {} //abstract class
void CompilerJVM::visitExp(Exp *t, bool silent) {} //abstract class

void CompilerJVM::visitProg(Prog *prog, bool silent)
{
  /* reset buffer and all the helper objects/variables */
  bufReset();
  this->storeMap.clear();
  this->localCount = 0;
  this->stackCurrent = 0;
  this->stackMax = 2;

  prog->liststmt_->accept(this, silent);
}

void CompilerJVM::visitSAss(SAss *s_ass, bool silent)
{
  visitIdent(s_ass->ident_, silent);
  s_ass->exp_->accept(this, silent);

  countStack(-1);

  /* Check if visitor was silenced. For more see Compiler class in Compiler.H */
  if (!silent) {
    /* Check if Ident wasn't already stored and increment local variables counter if it wasn't */
    if (this->storeMap.count(s_ass->ident_) == 0) {
      this->storeMap.insert(std::make_pair(s_ass->ident_, this->localCount));
      this->localCount++;
    }

    /* Get Ident index and append store command */
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
}

void CompilerJVM::visitSExp(SExp *s_exp, bool silent)
{
  countStack(1);

  /* Check if visitor was silenced. For more see Compiler class in Compiler.H */
  if (!silent) {
    bufAppend("\ngetstatic java/lang/System/out Ljava/io/PrintStream;\n");
  }

  s_exp->exp_->accept(this, silent);

  countStack(-2);

  /* Check if visitor was silenced. For more see Compiler class in Compiler.H */
  if (!silent) {
    bufAppend("invokevirtual java/io/PrintStream/println(I)V\n");
  }
}

/* This functions purpose is to count subexpressions stack usage and execute
 * them in the optimal order to minimize arithmetical operation stack usage */
void CompilerJVM::visitExpOpHelper(ExpOp *exp_op, bool silent, bool commutative) {
  int stackUsage1, stackUsage2;

  /* Save current stack counts to calculate stack usage by subexpressions */
  int stackCurrentSaved = this->stackCurrent;
  int stackMaxSaved = this->stackMax;

  if (exp_op->exp_1->stackUsage == -3) {
    /* Set stack counts to zero to get local stack usage in subexpressions */
    this->stackCurrent = 0;
    this->stackMax = 0;

    /* Silence first visitor - its purpose is to only count stack usage and nothing else */
    exp_op->exp_1->accept(this, true);
    exp_op->exp_1->stackUsage = this->stackMax;//remember first subexpression stack usage
  }

  if (exp_op->exp_2->stackUsage == -3) {
    this->stackCurrent = 0;
    this->stackMax = 0;

    exp_op->exp_2->accept(this, true);
    exp_op->exp_2->stackUsage = this->stackMax;//remember second subexpression stack usage
  }

  stackUsage1 = exp_op->exp_1->stackUsage;
  stackUsage2 = exp_op->exp_2->stackUsage;

  /* If the operation was silenced, then we don't want to execute the suboperations once again
   * to maintain linear complexity. Instead we can calculate op stack usage from subexpressions
   * stack usage and pass it up */
  if (silent) {
    this->stackCurrent = stackCurrentSaved;
    if (stackUsage1 > stackUsage2) {
      this->stackMax = (stackCurrentSaved + stackUsage1) > stackMaxSaved ? stackCurrentSaved + stackUsage1 : stackMaxSaved;
    }
    else if (stackUsage2 > stackUsage1) {
      this->stackMax = (stackCurrentSaved + stackUsage2) > stackMaxSaved ? stackCurrentSaved + stackUsage2 : stackMaxSaved;
    }
    else /* stackUsage1 == stackUsage2 */ {
      /* The one in this equation stands for the result of first expression being present on the stack
       * while executing the second expression incrementing the overall operation stack usage by one */
      this->stackMax = (stackCurrentSaved + stackUsage2 + 1) > stackMaxSaved ? stackCurrentSaved + stackUsage2 + 1 : stackMaxSaved;
    }
  }
  else {
    /* Restore stack counts */
    this->stackCurrent = stackCurrentSaved;
    this->stackMax = stackMaxSaved;

    /* Execute the command with bigger stack usage first and append swap if needed */
    if (stackUsage1 >= stackUsage2) {
      exp_op->exp_1->accept(this, silent);
      exp_op->exp_2->accept(this, silent);
    }
    else {
      exp_op->exp_2->accept(this, silent);
      exp_op->exp_1->accept(this, silent);

      /* Add swap command if the operation isn't commutative */
      if (!commutative) {
        bufAppend("swap\n");
      }
    }
  }
}

void CompilerJVM::visitExpAdd(ExpAdd *exp_add, bool silent)
{
  visitExpOpHelper(exp_add, silent, true);

  countStack(-1);

  /* Check if visitor was silenced. For more see Compiler class in Compiler.H */
  if (!silent) {
    bufAppend("iadd\n");
  }
}

void CompilerJVM::visitExpSub(ExpSub *exp_sub, bool silent)
{
  visitExpOpHelper(exp_sub, silent, false);

  countStack(-1);

  /* Check if visitor was silenced. For more see Compiler class in Compiler.H */
  if (!silent) {
    bufAppend("isub\n");
  }
}

void CompilerJVM::visitExpMul(ExpMul *exp_mul, bool silent)
{
  visitExpOpHelper(exp_mul, silent, true);

  countStack(-1);

  /* Check if visitor was silenced. For more see Compiler class in Compiler.H */
  if (!silent) {
    bufAppend("imul\n");
  }
}

void CompilerJVM::visitExpDiv(ExpDiv *exp_div, bool silent)
{
  visitExpOpHelper(exp_div, silent, false);

  countStack(-1);

  /* Check if visitor was silenced. For more see Compiler class in Compiler.H */
  if (!silent) {
    bufAppend("idiv\n");
  }
}

void CompilerJVM::visitExpLit(ExpLit *exp_lit, bool silent)
{
  visitInteger(exp_lit->integer_, silent);
}

void CompilerJVM::visitExpVar(ExpVar *exp_var, bool silent)
{
  visitIdent(exp_var->ident_, silent);

  countStack(1);

  /* Check if visitor was silenced. For more see Compiler class in Compiler.H */
  if (!silent) {
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
}

void CompilerJVM::visitListStmt(ListStmt *list_stmt, bool silent)
{
  for (ListStmt::iterator i = list_stmt->begin() ; i != list_stmt->end() ; ++i)
  {
    (*i)->accept(this, silent);
  }
}

void CompilerJVM::visitInteger(Integer x, bool silent)
{
  countStack(1);

  /* Check if visitor was silenced. For more see Compiler class in Compiler.H */
  if (!silent) {
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
}

void CompilerJVM::visitIdent(Ident x, bool silent) {}
