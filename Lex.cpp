//
// Created by sandeep dommaraju on 3/2/16.
//

#include "Lex.h"

Token Lex::scan() {
    Token token;
    char ch;
    string content = "";

    if(next_char == '\0')
        ch = inputfile.get();
    else
        ch = next_char;

    while(ch == ' ' || ch == '\t' || ch == '\n'){
        ch = inputfile.get();
    }

    if(ch == '/'){
        content += ch;
        ch = inputfile.get();
        if(ch == '/'){
            while(ch != '\n' && ch!= EOF){
                content += ch;
                ch = inputfile.get();
            }
            if(content.size() > 0){
                next_char = ch;
                token.setType(DELETE);
                token.setValue(content);
                return token;
            }
        }
    }

    while(ch != ' ' && ch != '\n' && getCharType(ch) == OPERATOR_SYMBOL) { //take care of comments
        content += ch;
        ch = inputfile.get();
    }

    if(content.size() > 0){
        next_char = ch;
        token.setType(OPERATOR);
        token.setValue(content);
        return token;
    }

    while(ch != ' ' && ch != '\n' && getCharType(ch) == DIGIT) { //take care of comments
        content += ch;
        ch = inputfile.get();
    }

    if(content.size() > 0){
        next_char = ch;
        token.setType(INTEGER);
        token.setValue(content);
        return token;
    }


    if(getCharType(ch) == LETTER){
        content += ch;
        ch = inputfile.get();
        while(getCharType(ch) == LETTER || getCharType(ch) == DIGIT || ch == '_') {
            content += ch;
            ch = inputfile.get();
        }
    }

    if(content.size() > 0){
        next_char = ch;
        if(isKeyWord(content)){
            token.setType(KEYWORD);
        }else{
            token.setType(IDENTIFIER);
        }
        token.setValue(content);
        return token;
    }

    if(ch == '('){
        content += ch;
        next_char = inputfile.get();
        token.setType(OPEN);
        token.setValue(content);
        return token;
    }

    if(ch == ')'){
        content += ch;
        next_char = inputfile.get();
        token.setType(CLOSE);
        token.setValue(content);
        return token;
    }

    if(ch == ';'){
        content += ch;
        next_char = inputfile.get();
        token.setType(SEMICOLON);
        token.setValue(content);
        return token;
    }

    if(ch == ','){
        content += ch;
        next_char = inputfile.get();
        token.setType(COMMA);
        token.setValue(content);
        return token;
    }

    if(ch == '\''){
        content += ch;
        ch = inputfile.get();
        while(ch != '\''){
            content += ch;
            ch = inputfile.get();
            if(ch == '\\'){
                ch = inputfile.get();
                if(ch == '\''){
                    //content += '\'';
                }else{
                    content += '\\';
                }
            }
        }
        if(content.size()>0){
            content += ch;
            next_char = inputfile.get();
            token.setType(STRING);
            token.setValue(content);
            return token;
        }
    }

    return token;
}

charType Lex::getCharType(char ch) {
    if(isOperatorSymbol(ch)){
        return OPERATOR_SYMBOL;
    }else if(isDigit(ch)){
        return DIGIT;
    }else if(isLetter(ch)){
        return LETTER;
    }else if(isPunctuation(ch)){
        return PUNCTUATION;
    }
    return OTHER;
}

bool Lex::isOperatorSymbol(char ch){
    char ops[] = {'+', '-', '*', '<', '>', '&', '.', '@', '/', ':', '=', '~', '|', '$', '!', '#', '%', '^', '_', '[', ']', '{', '}', '"', '`', '?' };
    for(int i=0; i<sizeof(ops)/sizeof(ops[0]); i++){
        if(ch == ops[i]) return true;
    }
    return false;
}

bool Lex::isDigit(char ch){
    return ch >= 48 && ch <= 57;
}

bool Lex::isLetter(char ch){
    return (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122);
}

bool Lex::isPunctuation(char ch){
    char ops[] = {'(', ')', ';', ','};
    for(int i=0; i<sizeof(ops)/sizeof(ops[0]); i++){
        if(ch == ops[i]) return true;
    }
    return false;
}

bool Lex::isKeyWord(string word) {
    string keywords[] = {"let", "where", "in", "fn", "and", "rec", "within", "aug", "not", "or", "gr", "ge", "ls", "le", "eq", "ne", "true", "false", "nil", "dummy"};
    for(int i=0; i<sizeof(keywords)/sizeof(keywords[0]); i++){
        if(word.compare(keywords[i]) == 0)
            return true;
    }
    return false;
}

