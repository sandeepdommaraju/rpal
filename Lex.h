//
// Created by sandeep dommaraju on 3/2/16.
//

#ifndef RPAL_LEX_H
#define RPAL_LEX_H

#include "Token.h"
#include<fstream>

class Identifier : public Token{

};

class Integer : public Token{

};

class Operator : public Token{

};

class String : public Token{

};

class Spaces : public Token {

};

class Comment : public Token{

};

enum charType {OPERATOR_SYMBOL, DIGIT, LETTER, PUNCTUATION, OTHER};

//string key_words[] = {"let", "where", "in", "fn", "and", "rec", "within", "aug", "not", "or", "gr", "ge", "ls", "le", "eq", "ne", "true", "false", "nil", "dummy"};

class Lex{

    public:
        Lex() : inputfile("none"), next_char('\0')
        {}

        Lex(string filename) {
                next_char = '\0';
                inputfile.open(filename);
                if(inputfile.fail()){
                        cout << "ERR: Failed to open: " + filename << endl;
                        exit(0);
                }
                cout << "OPENED file: " + filename << endl;
        }

        Token scan();

   private:
        ifstream inputfile;
        char next_char;

        charType getCharType(char);
        bool isOperatorSymbol(char);
        bool isDigit(char);
        bool isLetter(char);
        bool isPunctuation(char);
        bool isKeyWord(string);
};




#endif //RPAL_LEX_H
