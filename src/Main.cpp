#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "project.h"

/**
 * @brief Main Function
 * *Everything expected to happen will happen here and perform with abstraction in other files
 */


int main() {
    /*
        ? Part 1: Display file with name [filename], clean up file,
        ? tokenize all tokens and output to [outputFileName]
    */
    std::string filename = "finalp1.txt";
    //display input file
    std::cout << "Content of finalp1.txt: \n" << std::endl;
    displayFile(filename);

    //call function to clean up file based on rules and output to an output file
    std::string outputFileName = "finalp2.txt";
    cleanupFile(filename, outputFileName);

    /*
        ? Part 2: Validate the file [outputFileName] just written into and produce any error occurred
    */
    std::cout << " \t ************* \n\n";
    std::cout << "START COMPLING PROGRAM: \n";
    
    bool fileIsValid = false;
    
    std::cout << "\n";
    std::string originalFile = filename;
    fileIsValid = validateFile(originalFile, outputFileName);
    std::cout << "\n";

    /*
        ? Part 3: generate C++ code from the output file if no error occurred
    */

   std::string abstractFile = retrieveAbstractFile();
   if (fileIsValid) {
       std::cout << "Generating C++ code from abstract file: \n";
       codeGenerate(abstractFile);
       std::cout << "Compilation done with no error. \n";
   }
   
    return 0;
}