//
// Created by sandeep dommaraju on 3/2/16.
//

#ifndef RPAL_TOKEN_H
#define RPAL_TOKEN_H


#include<string>
#include<iostream>
#include<cstdlib>

using namespace std;

enum tokenType{IDENTIFIER, INTEGER, OPERATOR, STRING, OPEN, CLOSE, SEMICOLON, COMMA, DELETE, KEYWORD, NONE};

class Token {

public:

    Token() : type(NONE), val("none")
    {}

    Token(tokenType mType, string mVal) : type(mType), val(mVal)
    {}

    void setType(tokenType);
    void setValue(string);

    string getTypeStr();
    tokenType getType();
    string getValue();

private:

    tokenType type;
    string val;

};

#endif //RPAL_TOKEN_H};
