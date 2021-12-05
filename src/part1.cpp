#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "project.h"

//-------------------------------------------------------------------------------------------------------------
// Given the file "finalp1.txt":
//      Apply the following rules to this file and copy the new version in file “finalp2.txt”
//      a.	Any line/s or part of a line that begins with ** and ends with ** is considered 
//          as a comment line (i.e. lines #2,3,5, 11,13,15), remove them
//      b.	Any blank lines must be removed (i.e. line #12)
//      c.	Extra spaces in each line must be removed, 
//          Leave one space before and one after each token (example: line 8: a1 = 3 ; )
//-----------------------------------------------------------------------------------------------------------------

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
            /*
            * Keywords are probably misspelled, so we need to check them here
            * the indicator here is that it contains characters that are not a part of an identifier
            * that is a character is not a, b, c, d, w, f, and 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
            * also note that this keywords misspelled checking will also exclude all operators and punctuation
            * otherwise it should be an identifier
            */

            for (int j = 0; j < word.length(); j++) {
                //basically also case mentioned above and identifier are excluded from checking for a typo in keywords
                if (word[j] != 'a' && word[j] != 'b' && word[j] != 'c' && word[j] != 'd' && word[j] != 'w' && word[j] != 'f' 
                    && !isDigit(word[j]) && word[j] != '"' && word[j] != ',' && word[j] != '+' && word[j] != '-' && word[j] != '*'
                    && word[j] != '/' && word[j] != '=' && word[j] != ':' && word[j] != '(' && word[j] != ')' && word[j] != ';') {

                    *file += word + " ";
                    return;
                }
            }

            *file += word.substr(i,1);
        }
    }
}

bool containsSymbol(std::string word) {
    return word.find(":") != std::string::npos|| word.find("(") != std::string::npos 
        || word.find(")") != std::string::npos || word.find(";") != std::string::npos
        || word.find("\"") != std::string::npos || word.find(",") != std::string::npos;
}

void handleWord(std::string* file, std::string word) {
    std::string keyword;
    std::string otherWord = word;
    for (int i = 0; i < 6; i++) {
        /*
        The concern here is that write a keyword may involve in a longer word with it inside
        */
        size_t found = word.find(keywords[i]);
        if (found != std::string::npos && containsSymbol(word)) {
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
/*
 * This function will remove all the comments and blank lines
 * called handleWord() to separate words (separated by a whitespace) into tokens.
 */
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
        
        std::string formattedFile = formatFile(stringFile);
        outputFile << formattedFile;
        
        //close the file 
        inputFile.close();
        outputFile.close();
        
        std::cout << "============================================================ \n\n";
        std::cout << "Content of " << outputFileName << "\n" << std::endl;
        //then display the content of the new file newdata.txt
        displayFile(outputFileName);
    }
    catch (std::exception const& e) {
        std::cout << "An error occurred: " << e.what() << std::endl;
    }
}