#include <iostream>
#include <string>

/*
    Function headers
*/
void displayFile(std::string filename);
std::string formatFile(std::string fileString);
void handleKeyWord(std::string* file, std::string word);
void handleOtherWord(std::string* file, std::string otherWord);
void handleWord(std::string* file, std::string word);

void cleanupFile(std::string inputFileName, std::string outputFileName);

//This class is use as a blueprint to create a stack
//Stack of this class is supposed to hold the input string
class Stack {
	//two private fields: stack and counter
	private: 
    	
        std::string* stack;
    	int counter, maxSize;
        float threshold = 0.5;
    
    //operations on the stack
    public:
        Stack();
        ~Stack();
    	void clearStack();
    	bool isEmpty();
	    bool isFull();
	    void push(std::string element);
	    std::string pop();
        void resize();

	/*
    Customized methods to check elements in the stack
    - printStack(): print the stack with customized output lines
    - size(): inquire the size of the stack
    */
	
    void printStack(std::ofstream &outputFile);
    int size();
};

void validateFile(std::string filename);
