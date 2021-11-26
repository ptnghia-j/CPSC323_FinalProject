#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "unistd.h"
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
    
    //create a little delay effect with sleep
    sleep(1);
    std::cout << "\n";
    validateFile(outputFileName);
    std::cout << "DONE !!!" << std::endl;

    return 0;
}