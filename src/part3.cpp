#include <iostream>
#include <fstream>
#include "project.h"

/**
 * @brief 
 * * This file contains a simple function that generate the equivalent C++ code from the given original source code 
 * * in Pascal language, format follows a typical C++ formatting style, indentation (a tab) here is 4 spaces.
 * * There will be a space between each token to ease the generating process and enhance redability
 * 
 * * Even though print() in Pascal does not break into a new line, this C++ translation code will add a new line for each
 * * cout statement, return 0 is optional but is appended to the end of main() in this output
 * 
 * * Some initial settings are hard coded, and is considered a blueprint part of the new grammar rules, in an assumption 
 * * that they are the equivalence counterpart of the original Pascal code
 * 
 * * Codes components that are supported by the original Pascal code are supposed to be supported in the translation to C++ here
 */

/*
If there are no ERRORS, translate the program to a high-level language: C++
* The transformed Grammar based on the original grammar
<prog> ->#include <iostream> <new-line> using namespace std; <type> main() { <dec-list> <stat-list> return 0;}
<new-line> -> “\n”                                   
<type> -> int
<dec-list> -> <type> <dec>;
<dec> -> <identifier>,<dec>|<identifier>
<identifier> -> <letter>{<letter>|<digit>}
<stat-list> -> <stat>|<stat><stat-list>
<stat> -> <cout>|<assign>
<cout> -> cout<< <str><identifier><endl>;
<endl> -> endl | λ
<assign> -> <identifier>=<expr>;
<expr> -> <expr>+<term>|<expr>-<term>|<term>
<term> -> <term>*<factor>|<term>/<factor>|<factor>
<factor> -> <identifier>|<number>|(<expr>)
<number> -> <sign><digit>{<digit>}
<sign> -> +|-|λ
<digit> -> 0|1|…|9
<letter> -> a|b|c|d|w|f
*/

void codeGenerate(std::string abstractFileData) {
    std::string fileName;

    int fileNameStart = (int)abstractFileData.find("p") + 1;
    int fileNameEnd = (int)abstractFileData.find(";");

    fileName = abstractFileData.substr(fileNameStart, fileNameEnd - fileNameStart);

    std::string declarationList;
    int declarationListStart = (int)abstractFileData.find("v") + 1;
    int declarationListEnd = (int)abstractFileData.find("t");

    declarationList = abstractFileData.substr(declarationListStart, declarationListEnd - declarationListStart);

    std::string statementList;
    int statementListStart = (int)abstractFileData.find("t") + 1;
    int statementListEnd = (int)abstractFileData.find("e");

    statementList = abstractFileData.substr(statementListStart, statementListEnd - statementListStart);
    try {
        std::ofstream targetCodeOutput(fileName + ".cpp");
        if(!targetCodeOutput.is_open()) {
            throw std::runtime_error("Unable to generate the target program. Something went wrong.");
        }

        targetCodeOutput << "#include <iostream>\nusing namespace std;\nint main() {\n";
        targetCodeOutput << "\t";
        
        std::string dataType;
        int dataTypeStart = (int)declarationList.find(":") + 1;
        int dataTypeEnd = (int)declarationList.find(";", dataTypeStart + 1);

        dataType = declarationList.substr(dataTypeStart, dataTypeEnd - dataTypeStart);
        dataType == "i" ? targetCodeOutput << "int ": std::cout << " Invalid data type observed: " << dataType << std::endl;

        for (int i = 0; i < dataTypeStart - 1; i++) {
            //append each identifier to the target code, separated by a comma, leave a space before and after each comma
            declarationList[i] != ',' ? targetCodeOutput << declarationList[i] : targetCodeOutput << " , ";
        }
        targetCodeOutput << " ;\n";
        targetCodeOutput << "\t";

        bool doneOutputing = false;

        for (int i = 0; i < statementList.length(); i++) {
            if (statementList[i] == ';') {
                targetCodeOutput << " ;";
                targetCodeOutput << "\n\t";
            }  
            else if (statementList[i] == '=' || statementList[i] == '+' || statementList[i] == '-' 
                    || statementList[i] == '*' || statementList[i] == '/') {
                targetCodeOutput << " "  << statementList[i] << " ";
            } 
            else if (statementList[i] == '(') {
                targetCodeOutput << "( ";
            } 
            else if (statementList[i] == ')') {
                targetCodeOutput << " )";
            } 
            else if (statementList[i] == 'q'){
                doneOutputing = false;
                targetCodeOutput << "cout << ";
                i++;
                while (!doneOutputing) {
                    if(statementList[i] == 'o') {
                        targetCodeOutput << "\"value=\" ";
                    }
                    else if (statementList[i] == ',') {
                        targetCodeOutput << " << ";
                    }
                    else if (statementList[i] == ';') {
                        targetCodeOutput << " << endl;\n\t";
                        doneOutputing = true;
                        break;
                    } 
                    else if (statementList[i] == '(' || statementList[i] == ')') {
                        //add nothing
                    }
                    else {
                        targetCodeOutput << statementList[i];
                    }
                    i++;
                }
            }
            else {
                targetCodeOutput << statementList[i];
            }
        }

        //Finally append return 0 to the target code file followed by a close parenthesis
        targetCodeOutput << "return 0;\n}";

        //close the file
        targetCodeOutput.close(); 
    }
    catch(std::runtime_error &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}