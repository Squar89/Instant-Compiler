#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstddef>
#include <string>
#include "Parser.H"
#include "Compiler.H"
#include "CompilerJVM.H"

/* Usage instructions */
void usage() {
  printf("usage: ./insc_jvm file_name\n\n");
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

  /* Input file should end with .ins - this will transform it to .j */
  int fileNameLength = strlen(fileName);
  fileName[fileNameLength - 3] = 'j';
  fileName[fileNameLength - 2] = '\0';

  /* Extract directory and filename without it from provided file */
  std::string directory = std::string(fileName);
  std::string fileNameCropped;
  std::size_t found = directory.find_last_of("/");
  if (found == std::string::npos) {//directory not found
    directory = ". ";
    fileNameCropped = std::string(fileName);
  }
  else {
    fileNameCropped = directory.substr(found + 1);
    directory = directory.substr(0, found) + " ";
  }
  /* get rid of .j at the end of fileNameCropped */
  fileNameCropped = fileNameCropped.substr(0, fileNameCropped.length() - 2);

  /* Setup our compiler and compile parsed input */
  compiler = new CompilerJVM();
  /* Make sure that parse was successful */
  if (parseTree) {
    result = compiler->compile(parseTree, fileNameCropped);
    
    /* Open file with name transformed to .j and write result to it */
    std::ofstream compiledSourceFile (fileName);
    compiledSourceFile << result << std::endl;
    compiledSourceFile.close();

    /* Execute command generating .class file using freshly compiled .j file */
    std::string jasminCallString = "java -jar ./lib/jasmin.jar -d " + directory + std::string(fileName);
    const char *jasminCall = jasminCallString.c_str();
    system(jasminCall);

    return 0;
  }

  return 1;//parse was unsuccessful so we return 1
}