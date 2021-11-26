//-------------------------------------------------------------------------------------------------------------
// Given the file "finalp1.txt":
//      Apply the following rules to this file and copy the new version in file “finalp2.txt”
//      a.	Any line/s or part of a line that begins with ** and ends with ** is considered 
//          as a comment line (i.e. lines #2,3,5, 11,13,15), remove them
//      b.	Any blank lines must be removed (i.e. line #12)
//      c.	Extra spaces in each line must be removed, 
//          Leave one space before and one after each token (example: line 8: a1 = 3 ; )
//-----------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "project.h"

std::string keywords[6] = {"program", "begin", "end.", "var", "integer", "write"};

bool isDigit(char c) {
    return (c >= '0' && c <= '9');
}

void displayFile(std::string filename) {
    std::ifstream inputFile(filename);
    std::string line;
    if (inputFile.is_open()) {
        while(std::getline(inputFile, line)) {
            std::cout << line << std::endl;
        }
    } 
    else {
        std::cout << "Cannot open file " << filename << std::endl;
    }
    //close the file 
    inputFile.close();
}

void handleKeyWord(std::string* file, std::string word){
    switch(word[0]) {
        case 'i':
            *file += word;
            break;
        case 'p':    
        case 'w':
            *file += word + " ";
            break;
        case 'v':
        case 'b':
            *file += word + "\n";
            break;
        case 'e':
            *file += word;
            break;
        default:
            break;
    }
}

void handleOtherWord(std::string* file, std::string otherWord) {
    std::string word = otherWord;
    for (int i = 0; i < word.length(); i++) {
        if (word[i] == '"') {
            *file += word.substr(i,1);
            i++;
            //add everything between "" to the formattedFile
            while(word[i] != '"') {
                *file += word.substr(i,1);
                i++;
            }
            
            *file += word.substr(i,1);
        }
        else if (word[i] == ',' || word[i] == '+' || word[i] == '-' || word[i] == '*' || word[i] == '/' || word[i] == '=' || word[i] == ':') {
            *file += " " + word.substr(i,1) + " ";
        }
        else if (word[i] == '(') {
            *file += word.substr(i,1) + " ";
        }
        else if (word[i] == ')') {
            *file += " " + word.substr(i,1);
        }
        else if (word[i] == ';') {
            *file += " " + word.substr(i,1) + "\n";
        }
        else {
            *file += word.substr(i,1);
        }
    }
}


void handleWord(std::string* file, std::string word) {
    std::string keyword;
    std::string otherWord = word;
    for (int i = 0; i < 6; i++) {
        /*
        The concern here is that write a keyword may involve in a longer word with it inside
        */
        size_t found = word.find(keywords[i]);
        if (found != std::string::npos) {
            if (found == 0) {
                keyword = word.substr(found, keywords[i].length());
                handleKeyWord(file, keyword);
                otherWord = word.substr(found + keyword.length());
                handleOtherWord(file, otherWord);
            }
            else {
                otherWord = word.substr(0, found);
                handleOtherWord(file, otherWord);
                handleKeyWord(file, keyword);
                if (found + keyword.length() < word.length() - 1) {
                    otherWord = word.substr(found + keyword.length());
                    handleOtherWord(file, otherWord);
                }
            }
            return; 
        }
    }
    handleOtherWord(file, otherWord);
}

std::string formatFile(std::string fileString) {
    std::string fileWithNoComment = "";
    bool isComment = false;
    for (int i = 0; i < fileString.length(); i++) {
        if(isComment) {
            //check for the end of comment
            if(fileString[i] == '*' && fileString[i+1] == '*') {
                isComment = false;
                i += 2;
            }
            else {
                continue;
            }
        }
        else if (fileString[i] == '*' && fileString[i+1] == '*') {
            isComment = true;
            i += 2;
        }
        else {
            fileWithNoComment += fileString[i];
        }
    }

    std::string formattedFile;
    std::stringstream stream(fileWithNoComment);

    //a word separated by a space
    //a word may contains multiple tokens
    std::string word;
    
    while(stream >> word) {
        handleWord(&formattedFile, word);
    }
          
    return formattedFile;
}

void cleanupFile(std::string inputFileName, std::string outputFileName) {
    try {
        std::ifstream inputFile(inputFileName);
        if(!inputFile.is_open()) {
            throw std::runtime_error("Cannot open " + inputFileName);
        }

        std::ofstream outputFile(outputFileName);
        if (!outputFile.is_open()) {
            throw std::runtime_error("Cannot open " + outputFileName);
        }

        std::string line;
        std::string stringFile;

        while(std::getline(inputFile, line)) {
            stringFile += line;
        }
        
        //close the file 
        inputFile.close();

        std::string formattedFile = formatFile(stringFile);
        outputFile << formattedFile;

        outputFile.close();

        std::cout << " \t ************* \n\n";
        std::cout << "Content of " << outputFileName << "\n" << std::endl;
        //then display the content of the new file newdata.txt
        displayFile(outputFileName);
    }
    catch (std::exception const& e) {
        std::cout << "An error occurred: " << e.what() << std::endl;
    }
    
}
