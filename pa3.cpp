//
//  main.cpp
//  pa3
//
//  Created by Christian on 11/15/16.
//  Copyright © 2016 Christian. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "pa3.h"

using namespace std;

void updateMaxDepth (int & maxDepth, int currentDepth);

bool isLowerCase(string x);
bool isNumber(string x);

int hasEqualSign(string x);


    
//adds string to next spot in array
void NumberOf::addToArray(string x){
    //does a for loop that iterates up to the currentPoint, including the first point
    for (int i = 0; i <= currentPoint || currentPoint == 0; i++){
        
        //if it gets to the currentPoint iteration and no repeats, adds to array
        if (i == currentPoint){
            number[currentPoint] = x;
            currentPoint++;
            break;
        }
            
        //if finds repeat, breaks
        else if (x == number[i]){
            break;
        }
    }
    }
    
void NumberOf::print() {
    for (int i = 0; i <= currentPoint; i++){
        cout << number[i] << "  ";
    }
    cout << endl;
}


              
enum State {
    NEXT_WORD,
    FOR_VARIABLE,
    FOR_CONDITION,
    FOR_INCREMENT,
    FOR_BEGIN,
    FOR_END,
    LOWERCASE
};

int main(int argc, const char * argv[]) {
    //accepts user inputed file
    string input;
    cout << "Please enter a filename: ";
    cin >> input;
    
    
    //checks to make sure user-inputed file can be opened
    std::ifstream file(input.c_str());
    if(!file.is_open()) {
        std::cout << "The file could not be opened!" << std::endl;
        return 1;
    }
    
    State state = NEXT_WORD;
    
    //hold depths of for loop
    int currentDepth = -1;
    int maxDepth = 0;
    
    //holds number of BEGIN's, FOR's, and END's
    int numFors = 0;
    int numBegins = 0;
    int numEnds = 0;
    
    //holds arrays for all values
    NumberOf keywords;
    NumberOf identifiers;
    NumberOf constants;
    NumberOf operators;
    NumberOf delimeters;
    NumberOf syntaxErrors;

    for(;;) {
        
        //checks if the next piece of the file exists
        string nextWord;
        file >> nextWord;
        if (!file.good()) {
           break;
        }
        
        switch (state) {
            case NEXT_WORD:
                
                //checks if next word is FOR keyword
                if (nextWord == "FOR"){
                    state = FOR_VARIABLE;
                    keywords.addToArray("FOR");
                    currentDepth++;
                    updateMaxDepth(maxDepth, currentDepth);
                    numFors++;
                    break;
                }
                
                //checks if next word is END keyword
                if (nextWord == "END"){
                    currentDepth--;
                    keywords.addToArray("END");
                    numEnds++;
                    break;
                }
                
                //checks if word ends with semicolon
                if (nextWord[nextWord.length() - 1] == 59){
                    delimeters.addToArray(";");
                    nextWord = nextWord.substr(0, nextWord.length() - 2);
                }
                
                //checks if next word is possible identifier
                if (hasEqualSign(nextWord) > -1){
                    int x = hasEqualSign(nextWord);
                    if (x == 0 || x == nextWord.length() - 1){
                        syntaxErrors.addToArray(nextWord);
                        break;
                    }
                    
                    //adds identifier before and after equal sign to identifiers array
                    //adds = to operators array
                    cout << x << endl;
                    identifiers.addToArray(nextWord.substr(0,x));
                    identifiers.addToArray(nextWord.substr(x+1));
                    operators.addToArray(nextWord.substr(x,1));
                    break;
                }
                
                //checks if it is a lowercase word
                if (isLowerCase(nextWord)){
                    identifiers.addToArray(nextWord);
                    break;
                }
                
                //checks if it is a constant
                if (isNumber(nextWord)){
                    constants.addToArray(nextWord);
                    break;
                }

                //checks if next word is operator
                if (nextWord == "+" || nextWord == "-" || nextWord == "*" || nextWord == "/"){
                    operators.addToArray(nextWord);
                    break;
                }
                
                //adds anything not covered by the other cases to errors
                syntaxErrors.addToArray(nextWord);
                break;

            case FOR_VARIABLE:
                state = FOR_CONDITION;
                //checks if the piece following the FOR keyword is ("variable_name",
                if (nextWord.length() < 3){
                    //TODO: add error
                    syntaxErrors.addToArray(nextWord);
                    break;
                }
                if (nextWord[0] != '(' || nextWord[nextWord.length()-1] != ',') {
                    //TODO: add error
                    syntaxErrors.addToArray(nextWord);
                    break;
                }
                if (!isLowerCase(nextWord.substr(1,nextWord.length() - 2))){
                    syntaxErrors.addToArray(nextWord);
                    break;
                }
                
                //adds variable name to identifiers and adds , to delimeters and moves to next condition
                delimeters.addToArray(",");
                identifiers.addToArray(nextWord.substr(1, nextWord.length() - 2));
                break;
                
            case FOR_CONDITION:
                state = FOR_INCREMENT;
                //checks if doesn't end with ','
                if (nextWord[nextWord.length() - 1] != ','){
                    //TODO: add error
                    syntaxErrors.addToArray(nextWord);
                    break;
                }
                if (!isNumber(nextWord.substr(0,nextWord.length() - 1))){
                    syntaxErrors.addToArray(nextWord);
                    break;
                }
                
                //adds constant to constants and moves to next condition
                constants.addToArray(nextWord.substr(0, nextWord.length() - 1));
                break;
                
            case FOR_INCREMENT:
                state = FOR_BEGIN;
                //checks to make sure ends with ')'
                if (nextWord[nextWord.length() - 1] != ')' || nextWord.substr(0,nextWord.length() - 1) != "++"){
                    //TODO: add error
                    syntaxErrors.addToArray(nextWord);
                    break;
                }
                
                
                //adds operator to operators and moves to next condition
                operators.addToArray(nextWord.substr(0,nextWord.length() - 1));
                break;
                
            case FOR_BEGIN:
                state = NEXT_WORD;
                
                //checks to make sure "BEGIN" is spelled right
                if (nextWord != "BEGIN"){
                    syntaxErrors.addToArray(nextWord);
                    break;
                }
                
                numBegins++;
                //adds "BEGIN" to keywords, updates the max depth and moves to next condition
                keywords.addToArray("BEGIN");
                break;
                
            default:
                break;
        }
    }

    
    cout << "The depth of the nested loop(s) is: " << maxDepth << endl;
    cout << "Keywords: ";
    keywords.print();
    cout << "Identifiers: ";
    identifiers.print();
    cout << "Constants: ";
    constants.print();
    cout << "Operators: ";
    operators.print();
    cout << "Delimeters: ";
    delimeters.print();
    cout << "Syntax Error(s): ";
    syntaxErrors.print();
    
    if (numBegins != numEnds || numBegins != numFors){
        cout << "There is an error in the number of BEGIN's, FOR's, or END's" << endl;
    }

    return 0;
    
    
}

void updateMaxDepth(int & maxDepth, int currentDepth) {
    //updates max depth of the loop
    if (currentDepth > maxDepth){
        maxDepth = currentDepth;
    }
}

int hasEqualSign(string x){
    //checks to make sure string is composed of lower letters and equal signs
    for (int i = 0; i < x.length(); i++){
        if (x[i] == 61){
            return i;
        }
    }
    return -1;
}

bool isNumber(string x){
    //checks to make sure all the pieces of the strings are numbers
    for (int i = 0; i < x.length(); i++) {
        if (x[i] < 48 || x[i] > 57){
            return false;
        }
    }
    return true;
}

bool isLowerCase(string x){
    //checks to make sure string is composed of lower-case letters
    for (int i = 0; i < x.length(); i++){
        if (x[i] < 97 || x[i] > 122){
            return false;
        }
    }
    return true;
}

