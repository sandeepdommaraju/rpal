#include <iostream>
#include "Lex.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Hello, World!" << endl;

    string inputfile;

    if(argc == 2) {
        inputfile = argv[1];
        cout << "inputfile: " << inputfile + " !!" << endl;
    }

    Lex lex(inputfile);
    Token t = lex.scan();
    while(t.getType()!=NONE){
        cout << t.getTypeStr() << " : " << t.getValue() << endl;
        t = lex.scan();
    }

    return 0;
}