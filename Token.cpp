//
// Created by sandeep dommaraju on 3/2/16.
//

#include "Token.h"

/*Token::Token(string mType, string mVal) {
    type = mType;
    val = mVal;
}*/

void Token::setType(tokenType mType) {
    type = mType;
}

void Token::setValue(string mVal) {
    val = mVal;
}

tokenType Token::getType() {
    return type;
}

string Token::getTypeStr(){
    if(type==0){
        return "IDENTIFIER";
    }else if(type==1){
        return "INTEGER";
    }else if(type==2){
        return "OPERATOR";
    }else if(type==3){
        return "STRING";
    }else if(type==4){
        return "OPEN";
    }else if(type==5){
        return "CLOSE";
    }else if(type==6){
        return "SEMICOLON";
    }else if(type==7){
        return "COMMA";
    }else if(type==8){
        return "DELETE";
    }else if(type==9){
        return "KEYWORD";
    }
    return "NONE";
}

string Token::getValue() {
    return val;
}