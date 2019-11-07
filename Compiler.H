#ifndef COMPILER_HEADER
#define COMPILER_HEADER

#include "Absyn.H"

#define BUFFER_INITIAL 2000

class Compiler : public Visitor
{
public:
  char *compile(Visitable *v);

  void visitProgram(Program *p);
  void visitStmt(Stmt *p);
  void visitExp(Exp *p);
  void visitProg(Prog *p);
  void visitSAss(SAss *p);
  void visitSExp(SExp *p);
  void visitExpAdd(ExpAdd *p);
  void visitExpSub(ExpSub *p);
  void visitExpMul(ExpMul *p);
  void visitExpDiv(ExpDiv *p);
  void visitExpLit(ExpLit *p);
  void visitExpVar(ExpVar *p);
  void visitListStmt(ListStmt *p);

  void visitInteger(Integer x);
  void visitChar(Char x);
  void visitDouble(Double x);
  void visitString(String x);
  void visitIdent(Ident x);

/* buffer and functions related to buffer storing result */
protected:
  char *buf_;
  int cur_, buf_size;

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

//TODO probably some protected store map
//TODO stack counter?
};


#endif
