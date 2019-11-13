#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include "Parser.H"
#include "Compiler.H"
#include "CompilerLLVM.H"

/* Usage instructions */
void usage() {
  printf("usage: ./insc_llvm file_name\n\n");
  printf("\t--help\t\tDisplay this help message.\n");
  printf("\tfile name\t\tCompile this file.\n");
}

int main(int argc, char ** argv) {
  FILE *input;
  char *fileName = NULL;
  char *result;
  Compiler *compiler;

  if (argc > 1) {
    /* Set input source to specified file or display usage */
    fileName = argv[1];
    input = fopen(fileName, "r");
    if (!input) {
      usage();
      return 1;
    }
  }
  else {
    usage();
    return 1;
  }

  /* Parse the supplied input into a Program variable */
  Program *parseTree = pProgram(input);

  /* Input file should end with .ins - this will transform it to .ll */
  int fileNameLength = strlen(fileName);
  fileName[fileNameLength - 3] = 'l';
  fileName[fileNameLength - 2] = 'l';
  fileName[fileNameLength - 1] = '\0';

  /* Setup our compiler and compile parsed input */
  compiler = new CompilerLLVM();
  /* Make sure that parse was successful */
  if (parseTree) {
    result = compiler->compile(parseTree);

    /* Open file with name transformed to .ll and write result to it */
    std::ofstream compiledSourceFile (fileName);
    compiledSourceFile << result << std::endl;
    compiledSourceFile.close();

    /* Execute command generating helper.bc file using freshly compiled .ll file */
    std::string llvmasCallString = "llvm-as -o helper.bc " + std::string(fileName);
    const char *llvmasCall = llvmasCallString.c_str();
    system(llvmasCall);

    /* Change input file name into .bc */
    int fileNameLength = strlen(fileName);
    fileName[fileNameLength - 2] = 'b';
    fileName[fileNameLength - 1] = 'c';
    fileName[fileNameLength] = '\0';

    /* Compile our runtime helper library */
    std::string llvmasLibCallString = "llvm-as ./lib/runtime.ll";
    const char *llvmasLibCall = llvmasLibCallString.c_str();
    system(llvmasLibCall);
    
    /* Link it */
    std::string llvlinkCallString = "llvm-link -o " + std::string(fileName) + " helper.bc ./lib/runtime.bc";
    const char *llvlinkCall = llvlinkCallString.c_str();
    system(llvlinkCall);

    return 0;
  }

  return 1;//parse was unsuccessful so we return 1
}