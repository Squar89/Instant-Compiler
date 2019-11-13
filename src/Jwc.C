#include <cstdio>
#include <string>
#include "Parser.H"
#include "Compiler.H"
#include "CompilerJVM.H"
#include "CompilerLLVM.H"

/* option string lengths used for strncmp */
#define JVM_OPT_LENGTH 5
#define LLVM_OPT_LENGTH 6

/* option codes set by compilator user */
#define JVM_OPTION 1
#define LLVM_OPTION 2

/* usage instructions */
void usage() {
  printf("usage: ./jwc -jvm | -llvm (file name)\n\n");
  printf("\t--help\t\tDisplay this help message.\n");
  printf("\t-jvm OR -llvm Specify destination language\n");
  printf("\t(file name)\t\tParse content of file. Optional - if no file is provided stdin will be used as input source\n");
}

int main(int argc, char ** argv) {
  int destLanguage;
  FILE *input;
  char *fileName = NULL;
  char *result;
  Compiler *compiler;

  if (argc > 1) {
    /* set destination language */
    if (strncmp(argv[1], "-jvm", JVM_OPT_LENGTH) == 0) {
      destLanguage = JVM_OPTION;
    }
    else if (strncmp(argv[1], "-llvm", LLVM_OPT_LENGTH) == 0) {
      destLanguage = LLVM_OPTION;
    }
    else {/* destination language not specified in command line arguments */
      usage();
      return 1;
    }

    /* set input source to specified file or stdin otherwise */
    if (argc > 2) {
      fileName = argv[2];
      input = fopen(fileName, "r");
      if (!input) {
        usage();
        return 1;
      }
    }
    else {
      input = stdin;
    }
  }
  else {
    usage();
    return 1;
  }

  /* parse the supplied input into a Program variable */
  Program *parseTree = pProgram(input);

  /* Setup our compiler and compile parsed input */
  if (destLanguage == JVM_OPTION) {
    compiler = new CompilerJVM();
  }
  else /* destLanguage == LLVM_OPTION */ {
    compiler = new CompilerLLVM();
  }

  /* Make sure that parse was successful */
  if (parseTree) {
    result = compiler->compile(parseTree);
    printf("%s", result);

    return 0;
  }

  return 1;//parse was unsuccessful so we return 1
}