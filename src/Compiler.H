#ifndef COMPILER_HEADER
#define COMPILER_HEADER

#include <cstring>
#include "Absyn.H"

#define BUFFER_INITIAL 2000

class Compiler : public Visitor
{
public:
  virtual char *compile(Visitable *v) = 0;

  /* Visitor functions are extended by silent boolean.
   * Silenced visitor only counts stack usage and doesn't change the enviroment in any other way.
   * This is implemented so a compiler can optimize order in which expressions are executed.
   * For example by knowing which subexpression from the two in add will require more stack space
   * we can execute it first and minimize stack space used by add operation.
   * Furthermore this can be extended to more than stack optimizing - whenever compiler needs to
   * just "take a peek" into an expression without really executing it. */
  virtual void visitProgram(Program *p, bool silent) = 0;
  virtual void visitStmt(Stmt *p, bool silent) = 0;
  virtual void visitExp(Exp *p, bool silent) = 0;
  virtual void visitProg(Prog *p, bool silent) = 0;
  virtual void visitSAss(SAss *p, bool silent) = 0;
  virtual void visitSExp(SExp *p, bool silent) = 0;
  virtual void visitExpAdd(ExpAdd *p, bool silent) = 0;
  virtual void visitExpSub(ExpSub *p, bool silent) = 0;
  virtual void visitExpMul(ExpMul *p, bool silent) = 0;
  virtual void visitExpDiv(ExpDiv *p, bool silent) = 0;
  virtual void visitExpLit(ExpLit *p, bool silent) = 0;
  virtual void visitExpVar(ExpVar *p, bool silent) = 0;
  virtual void visitListStmt(ListStmt *p, bool silent) = 0;
  virtual void visitInteger(Integer x, bool silent) = 0;
  virtual void visitIdent(Ident x, bool silent) = 0;

/* buffer and functions related to buffer storing result */
protected:
  char *buf_, *result;
  int cur_, buf_size;

  char *getResult(const char *header, const char *footer)
  {
    int lenHeader, lenFooter, resultSize;
    lenHeader = strlen(header);
    lenFooter = strlen(footer);
    resultSize = lenHeader + cur_ + lenFooter;

    result = (char *) malloc(resultSize);
    if (!result) {
      fprintf(stderr, "Error: Out of memory while allocating buffer!\n");
      exit(1);
    }
    memset(result, 0, resultSize);

    strcpy(result, header);
    strcat(result, buf_);
    strcat(result, footer);

    return result;
  }

  void inline bufAppend(const char *s)
  {
    int len = strlen(s);
    while (cur_ + len >= buf_size)
    {
      buf_size *= 2; /* Double the buffer size */
    }
    resizeBuffer();
    for(int n = 0; n < len; n++)
    {
      buf_[cur_ + n] = s[n];
    }
    cur_ += len;
    buf_[cur_] = 0;
  }

  void inline bufAppend(const char c)
  {
    if (cur_ >= buf_size)
    {
      buf_size *= 2; /* Double the buffer size */
      resizeBuffer();
    }
    buf_[cur_] = c;
    cur_++;
    buf_[cur_] = 0;
  }

  void inline bufAppend(String str)
  {
    const char *s = str.c_str();
    bufAppend(s);
  }

  void inline bufReset(void)
  {
    if (buf_) free(buf_);
    buf_size = BUFFER_INITIAL;
    buf_ = (char *) malloc(buf_size);
    if (!buf_) {
      fprintf(stderr, "Error: Out of memory while allocating buffer!\n");
      exit(1);
    }
    memset(buf_, 0, buf_size);
    cur_ = 0;
  }

  void inline resizeBuffer(void)
  {
    char *temp = (char *) malloc(buf_size);
    if (!temp)
    {
      fprintf(stderr, "Error: Out of memory while attempting to grow buffer!\n");
      exit(1);
    }
    if (buf_)
    {
      strcpy(temp, buf_);
      free(buf_);
    }
    buf_ = temp;
  }
};

#endif
