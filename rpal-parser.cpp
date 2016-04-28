#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <queue>

using namespace std;

bool output = false;
bool debug = false;

/**
*   Token START
*/

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


/**
*   Lex START
*/

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
                inputfile.open(filename.c_str());
                if(inputfile.fail()){
                        //cout << "ERR: Failed to open: " + filename << endl;
                        exit(0);
                }
                //cout << "OPENED file: " + filename << endl;
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


/**
*   Parser START
*/

class MTreeNode{
    Token token;
    vector<MTreeNode> children;
};

class TreeNode{
public:
    string val;
    TreeNode* left;
    TreeNode* right;
};

class Parser{
private:
    Lex* lex;
    Token nextToken;
    stack<TreeNode*> _stack;
public:

    Parser(string inputfile){
        lex = new Lex(inputfile);
        nextToken = lex->scan();
	while(nextToken.getType()==DELETE){
            nextToken = lex->scan();
        }
    }

    void Read(Token t);
    void BuildTree(string root, int N);
    void preorderTraversal(TreeNode* root, int dots);
    TreeNode* top();

    //standardize API
    void standardize_AST(TreeNode* root);

    TreeNode* standardize_LET(TreeNode* root);
    TreeNode* standardize_WHERE(TreeNode* root);
    TreeNode* standardize_WITHIN(TreeNode* root);
    TreeNode* standardize_FUNCFORM(TreeNode* root);
    TreeNode* standardize_AND(TreeNode* root);
    TreeNode* standardize_AT(TreeNode* root);
    TreeNode* standardize_REC(TreeNode* root);

    //Expressions
    void E();
    void Ew();

    //Tuple Expressions
    void T();
    void Ta();
    void Tc();

    //Boolean Expressions
    void B();
    void Bt();
    void Bs();
    void Bp();


    //Arithmetic Expressions
    void A();
    void At();
    void Af();
    void Ap();


    //Rators and Rands
    void R();
    void Rn();


    //Definitions
    void D();
    void Da();
    void Dr();
    void Db();

    //Variables
    void Vb();
    void Vl();



private:
};

//bool debug = false;
bool empty = true;

//char ops[] = {'+', '-', '*', '<', '>', '&', '.', '@', '/', ':', '=', '~', '|', '$', '!', '#', '%', '^', '_', '[', ']', '{', '}', '"', '`', '?' };
//string keywords[] = {"let", "where", "in", "fn", "and", "rec", "within", "aug", "not", "or", "gr", "ge", "ls", "le", "eq", "ne", "true", "false", "nil", "dummy"};


void Parser::Read(Token expectedToken) {

    if(nextToken.getType()!= expectedToken.getType()){
        //cout << "ERROR: Token mismatch while parsing!! expected: " + expectedToken.getTypeStr() + " found: " + nextToken.getTypeStr() << endl;
        exit(0);
    }

    if(nextToken.getType() == IDENTIFIER || nextToken.getType() == INTEGER || nextToken.getType() == STRING){
        if(nextToken.getType()==IDENTIFIER){
            BuildTree("<ID:" + nextToken.getValue()+">", 0);
        }else if(nextToken.getType()==INTEGER){
            BuildTree("<INT:" + nextToken.getValue()+">", 0);
        }else if(nextToken.getType()==STRING){
            BuildTree("<STR:" + nextToken.getValue()+">", 0);
        }
        //BuildTree(nextToken.getValue(), 0);
    }

    nextToken = lex->scan();
    while(nextToken.getType()==DELETE){
        nextToken = lex->scan();
    }
}

void Parser::BuildTree(string root, int N) {

    TreeNode* prev= NULL;
    for(int i=1; i<=N; i++){
        TreeNode* curr = _stack.top();
        if(i>1){
            curr->right = prev;
        }
        prev = curr;
        _stack.pop();
    }

    TreeNode* node = new TreeNode();
    node->val = root;
    if(N>0){
        node->left = prev;
    }

    _stack.push(node);
}


void Parser::preorderTraversal(TreeNode* root, int dots){
    if(root == NULL) return;
    for(int i=1; i<=dots; i++)
        cout << ".";
    cout << root->val << endl;
    preorderTraversal(root->left, dots+1);
    preorderTraversal(root->right, dots); //right child is a sibling!!
}

TreeNode* Parser::top() {
    return _stack.top();
}

///////     EXPRESSIONS     ////////
/**
 *   E  ->  'let' D 'in' E      =>  'let'
 *      ->  'fn' Vb+ '.' E      =>  'lambda'
 *      ->  Ew;
 */
void Parser::E(){
    if(nextToken.getValue().compare("let")==0){
        Read(Token(KEYWORD, "let"));
        D();
        Read(Token(KEYWORD, "in"));
        E();
        BuildTree("let", 2);
    }else if(nextToken.getValue().compare("fn")==0){
        Read(Token(KEYWORD, "fn"));
        Vb();
        int N = 1;
        while(nextToken.getType() == IDENTIFIER || nextToken.getType() == OPEN){
            Vb();
            N++;
        }
        Read(Token(OPERATOR, "."));
        E();
        BuildTree("lambda", N + 1);
    }else{
        Ew();
        //Read(Token(SEMICOLON, ";"));
    }
}

/**
 *    Ew   ->   T 'where' Dr    => 'where'
 *         ->   T;
 */
void Parser::Ew(){
    T();
    if(nextToken.getValue().compare("where")==0){
        Read(Token(KEYWORD, "where"));
        Dr();
        BuildTree("where", 2);
    }
}

///////     TUPLE EXPRESSIONS     ////////

/**
 *
 *  T   ->  Ta ( ',' Ta )+      =>  'tau'
 *      ->  Ta;
 *
 */
void Parser::T(){
    Ta();
    int N = 0;
    while(nextToken.getValue().compare(",") == 0){
        Read(Token(COMMA, ","));
        Ta();
        N++;
    }
    if(N>0)
        BuildTree("tau", N+1);
}

/**
 *  Ta  ->  Ta 'aug' Tc     => 'aug'
 *      ->  Tc;
 */
void Parser::Ta(){
    Tc();
    while(nextToken.getValue().compare("aug")==0){
        Read(Token(KEYWORD, "aug"));
        Tc();
        BuildTree("aug", 2);
    }
}

/**
 *  Tc  ->  B '->' Tc '|' Tc
 *      ->  B;
 */
void Parser::Tc(){
    B();
    if(nextToken.getValue().compare("->")==0){
        Read(Token(OPERATOR, "->"));
        Tc();
        Read(Token(OPERATOR, "|"));
        Tc();
        BuildTree("->", 3);
    }
}

///////     BOOLEAN EXPRESSIONS     ////////
/**
 *  B  -> B 'or' Bt     =>  'or'
 *     -> Bt;
 */
void Parser::B(){
    Bt();
    while(nextToken.getValue().compare("or")==0){
        Read(Token(KEYWORD, "or"));
        Bt();
        BuildTree("or", 2);
    }
}

/**
 *  Bt  -> Bt '&' Bs    => '&'
 *      -> Bs;
 */
void Parser::Bt(){
    Bs();
    while(nextToken.getValue().compare("&")==0){
        Read(Token(OPERATOR, "&"));
        Bs();
        BuildTree("&", 2);
    }
}

/**
 *  Bs  -> 'not' Bp     => 'not'
 *      -> Bp;
 */
void Parser::Bs(){
    if(nextToken.getValue().compare("not")==0){
        Read(Token(KEYWORD, "not"));
        Bp();
        BuildTree("not", 1);
    }else{
        Bp();
    }
}

/**
 *  Bp  -> A ( 'gr' | '>' ) A   => 'gr'
 *      -> A ( 'ge' | '>=') A   => 'ge'
 *      -> A ( 'ls' | '<' ) A   => 'ls'
 *      -> A ( 'le' | '<=') A   => 'le'
 *      -> A 'eq' A             => 'eq'
 *      -> A 'ne' A             => 'ne'
 *      -> A
 */
void Parser::Bp(){
    A();
    if(nextToken.getValue().compare("gr")==0 || nextToken.getValue().compare(">")==0){
        if(nextToken.getValue().compare("gr")==0){
            Read(Token(KEYWORD, "gr"));
        }else{
            Read(Token(OPERATOR, ">"));
        }
        A();
        BuildTree("gr", 2);
    }else if(nextToken.getValue().compare("ge")==0 || nextToken.getValue().compare(">=")==0){
        if(nextToken.getValue().compare("ge")==0){
            Read(Token(KEYWORD, "ge"));
        }else{
            Read(Token(OPERATOR, ">="));
        }
        A();
        BuildTree("ge", 2);
    }else if(nextToken.getValue().compare("ls")==0 || nextToken.getValue().compare("<")==0){
        if(nextToken.getValue().compare("ls")==0 ){
            Read(Token(KEYWORD, "ls"));
        }else{
            Read(Token(OPERATOR, "<"));
        }
        A();
        BuildTree("ls", 2);
    }else if(nextToken.getValue().compare("le")==0 || nextToken.getValue().compare("<=")==0){
        if(nextToken.getValue().compare("le")==0 ){
            Read(Token(KEYWORD, "le"));
        }else{
            Read(Token(OPERATOR, "<="));
        }
        A();
        BuildTree("le", 2);
    }else if(nextToken.getValue().compare("eq")==0){
        Read(Token(KEYWORD, "eq"));
        A();
        BuildTree("eq", 2);
    }else if(nextToken.getValue().compare("ne")==0){
        Read(Token(KEYWORD, "ne"));
        A();
        BuildTree("ne", 2);
    }
}

///////     ARITHMETIC EXPRESSIONS     ////////
/**
 *  A   ->  A '+' At    =>  '+'
 *      ->  A '-' At    =>  '-'
 *      ->    '+' At
 *      ->    '-' At    =>  'neg'
 *      ->  At;
 */
void Parser::A(){
    if(nextToken.getValue().compare("+")==0){
        Read(Token(OPERATOR, "+"));
        At();
    }else if(nextToken.getValue().compare("-")==0){
        Read(Token(OPERATOR, "-"));
        At();
        BuildTree("neg", 1);
    }else{
        At();
    }

    while(nextToken.getValue().compare("+")==0 || nextToken.getValue().compare("-")==0){
        if(nextToken.getValue().compare("+")==0){
            Read(Token(OPERATOR, "+"));
            At();
            BuildTree("+", 2);
        }else{
            Read(Token(OPERATOR, "-"));
            At();
            BuildTree("-", 2);
        }
    }
}

/**
 *  At  ->  At '*' Af   =>  '*'
 *      ->  At '/' Af   =>  '/'
 *      ->  Af;
 */
void Parser::At(){
    Af();
    while(nextToken.getValue().compare("*")==0 || nextToken.getValue().compare("/")==0){
        if(nextToken.getValue().compare("*")==0){
            Read(Token(OPERATOR, "*"));
            Af();
            BuildTree("*", 2);
        }else{
            Read(Token(OPERATOR, "/"));
            Af();
            BuildTree("/", 2);
        }
    }
}

/**
 *  Af  ->  Ap '**' Af  =>  '**'
 *      ->  Ap;
 */
void Parser::Af(){
    Ap();
    if(nextToken.getValue().compare("**")==0){
        Read(Token(OPERATOR, "**"));
        Af();
        BuildTree("**", 2);
    }
}

/**
 *  Ap  ->  Ap '@' '<IDENTIFIER>' R     => '@'
 *      ->  R;
 */
void Parser::Ap(){
    R();
    while(nextToken.getValue().compare("@")==0){
        Read(Token(OPERATOR, "@"));
        Read(Token(IDENTIFIER, "FOO")); //TODO check
        R();
        BuildTree("@", 3);
    }
}

///////     RATORS AND RANDS     ////////
/**
 *  R   ->  R Rn    => 'gamma'
 *      ->  Rn;
 */
void Parser::R(){
    Rn();
    while(nextToken.getType()==IDENTIFIER || nextToken.getType()==INTEGER || nextToken.getType()==STRING
            || nextToken.getValue().compare("true")==0 || nextToken.getValue().compare("false")==0 || nextToken.getValue().compare("nil")==0
            || nextToken.getValue().compare("(")==0 || nextToken.getValue().compare("dummy")==0){
        Rn();
        BuildTree("gamma", 2);
    }
}

/**
 *  Rn  ->  '<IDENTIFIER>'
 *      ->  '<INTEGER>'
 *      ->  '<STRING>'
 *      ->  'true'          =>  'true'
 *      ->  'false'         =>  'false'
 *      ->  'nil'           =>  'nil'
 *      ->  '(' E ')'
 *      ->  'dummy'         =>  'dummy';
 */
void Parser::Rn(){
    if(nextToken.getType()==IDENTIFIER){
        Read(Token(IDENTIFIER, "FOO"));
    }else if(nextToken.getType()==INTEGER){
        Read(Token(INTEGER, "FOO"));
    }else if(nextToken.getType()==STRING){
        Read(Token(STRING, "FOO"));
    }else if(nextToken.getValue().compare("true")==0){
        Read(Token(KEYWORD, "true"));
        BuildTree("<true>", 0);
    }else if(nextToken.getValue().compare("false")==0){
        Read(Token(KEYWORD, "false"));
        BuildTree("<false>", 0);
    }else if(nextToken.getValue().compare("nil")==0){
        Read(Token(KEYWORD, "nil"));
        BuildTree("<nil>", 0);
    }else if(nextToken.getValue().compare("(")==0){
        Read(Token(OPEN, "("));
        E();
        Read(Token(CLOSE, ")"));
    }else if(nextToken.getValue().compare("dummy")==0){
        Read(Token(KEYWORD, "dummy"));
        BuildTree("<dummy>", 0);
    }
}

///////     DEFINITIONS     ////////
/**
 *  D   ->  Da 'within' D       =>  'within'
 *      ->  Da;
 */
void Parser::D(){
    Da();
    if(nextToken.getValue().compare("within")==0){
        Read(Token(KEYWORD, "within"));
        D();
        BuildTree("within", 2);
    }
}

/**
 *  Da  ->  Dr ( 'and' Dr )+    =>  'and'
 *      ->  Dr;
 */
void Parser::Da(){
    Dr();
    int N = 0;
    while(nextToken.getValue().compare("and")==0){
        Read(Token(KEYWORD, "and"));
        Dr();
        N++;
    }

    if(N > 0)
        BuildTree("and", N+1);
}

/**
 *  Dr  ->  'rec' Db        =>  'rec'
 *      ->  Db;
 */
void Parser::Dr(){
    if(nextToken.getValue().compare("rec")==0){
        Read(Token(KEYWORD, "rec"));
        Db();
        BuildTree("rec", 1);
    }else{
        Db();
    }
}

/**
 *  Db  ->  Vl '=' E                    =>  '='
 *      ->  '<IDENTIFIER>' Vb+ '=' E     =>  'fcn_form'
 *      ->  '(' D ')' ;
 */
void Parser::Db(){
    if(nextToken.getValue().compare("(")==0){
        Read(Token(OPEN, "("));
        D();
        Read(Token(CLOSE, ")"));
    }else if(nextToken.getType()==IDENTIFIER){
        Read(Token(IDENTIFIER, "FOO"));
        if(nextToken.getValue().compare(",")==0 || nextToken.getValue().compare("=")==0){ //TODO IMPORTANT CHECK
            Vl(); //The first IDENTIFIER in Vl() is already read here!!
            Read(Token(OPERATOR, "="));
            E();
            BuildTree("=", 2);
        }else{
            Vb();
            int N=1;
            while(nextToken.getType()==IDENTIFIER || nextToken.getValue().compare("(")==0){
                Vb();
                N++;
            }
            Read(Token(OPERATOR, "="));
            E();
            BuildTree("function_form", N + 2);
        }
    }
}
///////     VARIABLES     ////////
/**
 *  Vb  ->  '<IDENTIFIER>'
 *      ->  '(' Vl ')'
 *      ->  '(' ')'             =>  '()'
 */
void Parser::Vb(){
    if(nextToken.getType()==IDENTIFIER){
        Read(Token(IDENTIFIER, "FOO"));
    }else {
        Read(Token(OPEN, "("));
        if(nextToken.getValue().compare(")")==0){
            Read(Token(CLOSE, ")"));
            BuildTree("()", 2);
        }else{
            Read(Token(IDENTIFIER, "FOO"));
            Vl();
            Read(Token(CLOSE, ")"));
        }
    }
}

/**
 *  Vl  ->  '<IDENTIFIER>' list ','     =>  ','?;
 */
void Parser::Vl(){
    int N = 1;
    while(nextToken.getValue().compare(",")==0){
        Read(Token(COMMA, ","));
        Read(Token(IDENTIFIER, "FOO"));
        N++;
    }
    if(N>1)
        BuildTree(",", N);
}

/**
 * standardize AST starts here
 */

//post order traversal while standardizing
void Parser::standardize_AST(TreeNode *root) {

    if(root==NULL) return;

    if(root->left!=NULL)
        standardize_AST(root->left);

    if(root->right!=NULL)
        standardize_AST(root->right);

    if(root->val.compare("let")==0)
        standardize_LET(root);
    else if(root->val.compare("where")==0)
        standardize_WHERE(root);
    else if(root->val.compare("within")==0)
        standardize_WITHIN(root);
    else if(root->val.compare("function_form")==0)
        standardize_FUNCFORM(root);
    else if(root->val.compare("and")==0)
        standardize_AND(root);
    else if(root->val.compare("@")==0)
        standardize_AT(root);
    else if(root->val.compare("rec")==0)
        standardize_REC(root);
    //else
    //    cout << "ERR: found undefined node type while standardizing AST" << endl;
}

//LET
TreeNode* Parser::standardize_LET(TreeNode *root) {
    if(root==NULL || root->left==NULL || root->left->right==NULL || root->left->left==NULL || root->left->left->right==NULL){
        //cout << "let subtree is not proper to standardize_LET" << endl;
        return NULL;
    }

    if(root->val.compare("let")!=0){
        //cout << "let AST does not contain let" << endl;
        return NULL;
    }

    if(root->left->val.compare("=")!=0){
        //cout << "let AST does not contain equals" << endl;
        return NULL;
    }

    root->val="gamma";
    TreeNode* eq = root->left;
    TreeNode* P = eq->right;

    eq->val="lambda";

    TreeNode* E = eq->left->right;
    eq->left->right = P;
    root->left->right = E;
    return root;
}

//WHERE
TreeNode* Parser::standardize_WHERE(TreeNode *root) {
    if(root==NULL || root->left==NULL || root->left->right==NULL || root->left->right->left==NULL || root->left->right->left->right==NULL){
        if(debug) cout << "where subtree is not proper to standardize_WHERE" << endl;
        return NULL;
    }

    if(root->val.compare("where")!=0){
        if(debug) cout << "where AST does not contain where" << endl;
        return NULL;
    }

    if(root->left->right->val.compare("=")!=0){
        if(debug) cout << "where does not contain equals" << endl;
        return NULL;
    }

    //change to let node
    root->val="let";
    TreeNode* P = root->left;
    TreeNode* eq = root->left->right;
    root->left = eq;
    eq->right = P;
    P->right = NULL;
    return standardize_LET(root);
}


//WITHIN
TreeNode* Parser::standardize_WITHIN(TreeNode *root) {

    if(root==NULL || root->left==NULL || root->left->right==NULL || root->left->left==NULL || root->left->left->right==NULL || root->left->right->left==NULL || root->left->right->left->right==NULL){
        if(debug) cout << "within subtree is not proper to standardize_WITHIN" << endl;
        return NULL;
    }

    if(root->val.compare("within")!=0){
        if(debug) cout << "within AST does not contain within" << endl;
        return NULL;
    }

    if(root->left->val.compare("=")!=0 || root->left->right->val.compare("=")!=0){
        if(debug) cout << "within does not contain equals" << endl;
        return NULL;
    }

    /*TreeNode* x1 = root->left->left;
    TreeNode* e1 = root->left->left->right;
    TreeNode* x2 = root->left->right->left;
    TreeNode* e2 = root->left->right->left->right;
    TreeNode* leftEq = root->left;
    TreeNode* rightEq = root->right;

    root->val="=";
    root->left=x2;
    root->left->right = rightEq;
    root->left->right->val="gamma";
    root->left->right->left = leftEq;
    root->left->right->left->right = e1;
    root->left->right->left->val="lambda";
    root->left->right->left->left->right=e2;
     */

    TreeNode* leq = root->left;
    TreeNode* req = leq->right;
    TreeNode* X1 = leq->left;
    TreeNode* E1 = X1->right;
    TreeNode* X2 = req->left;
    TreeNode* E2 = X2->right;

    root->val = "=";
    root->left = X2;
    req->val = "gamma";
    X2->right = req;

    TreeNode* lam = new TreeNode();
    lam->val = "lambda";
    req->left = lam;
    lam->right = E1;
    lam->left = X1;
    X1->right = E2;

    return root;

}

//FUNCFORM
TreeNode* Parser::standardize_FUNCFORM(TreeNode *root) {
    if(root==NULL || root->left==NULL || root->left->right==NULL || root->left->right->right==NULL){
        if(debug) cout << "funcform subtree is not proper to standardize_FUNCFORM" << endl;
        return NULL;
    }

    if(root->val.compare("function_form")!=0){
        if(debug) cout << "funcform AST does not contain function_form" << endl;
        return NULL;
    }

    TreeNode* P = root->left;
    TreeNode* V = root->left->right;

    root->val = "=";

    while(V->right!=NULL){
        TreeNode* lam = new TreeNode();
        lam->val="lambda";
        lam->left=V;

        P->right = lam;
        P = V;
        V = V->right;
    }

    P->right = V;

    //TODO-Doubt standardize V before returning?
    //standardize_AST(root);

    return root;
}

//AND
TreeNode* Parser::standardize_AND(TreeNode *root) {
    if(root==NULL || root->left==NULL || root->left->right==NULL){
        if(debug) cout << "and subtree is not proper to standardize_AND" << endl;
        return NULL;
    }
    if(root->val.compare("and")!=0){
        if(debug) cout << "and AST does not contain and" << endl;
        return NULL;
    }
    if(root->left->val.compare("=")!=0 || root->left->right->val.compare("=")!=0){
        if(debug) cout << "left/right child is not = to standardize_AND" << endl;
        return NULL;
    }

    TreeNode* eq = root->left;
    if(eq->left==NULL || eq->left->right==NULL){
        if(debug) cout << "not even a single assignment/binding found to standardize_AND" << endl;
        return NULL;
    }

    root->val = "=";
    TreeNode* comma = new TreeNode();
    TreeNode* tau = new TreeNode();
    comma->val = ",";
    tau->val = "tau";

    root->left = comma;
    root->left->right = tau;

    comma->left = eq->left;
    tau->left = eq->left->right;

    comma = comma->left;
    tau = tau->left;

    eq = eq->right;

    while(eq!=NULL){
        comma->right = eq->left;
        tau->right = eq->left->right;

        if(comma->right==NULL || tau->right==NULL){
            if(debug) cout << "one child of equal is NULL to standardize_AND" << endl;
            return NULL;
        }

        comma = comma->right;
        tau = tau->right;

        eq = eq->right;
    }
    comma->right = NULL;
    return root;
}

//AT
TreeNode* Parser::standardize_AT(TreeNode *root) {
    if(root==NULL || root->left==NULL || root->left->right==NULL){
        if(debug) cout << "at subtree is not proper to standardize_AT" << endl;
        return NULL;
    }
    if(root->val.compare("@")!=0){
        if(debug) cout << "at AST does not contain at" << endl;
        return NULL;
    }
    root->val = "gamma";
    TreeNode* E1 = root->left;
    TreeNode* N =  E1->right;
    TreeNode* E2 = N->right;

    if(E1==NULL || E2==NULL || N==NULL){
        if(debug) cout << "E1/N/E2 is null to standardize_AT" << endl;
        return NULL;
    }

    TreeNode* gam = new TreeNode();
    gam->val = "gamma";

    root->left = gam;
    gam->right = E2;

    gam->left = N;
    N->right = E1;
    E1->right = NULL;

    return root;
}

//REC
TreeNode* Parser::standardize_REC(TreeNode *root) {
    if(root==NULL || root->left==NULL || root->left->left==NULL || root->left->left->right==NULL){
        if(debug) cout << "rec subtree is not proper to standardize_REC" << endl;
        return NULL;
    }
    if(root->val.compare("rec")!=0){
        if(debug) cout << "at AST does not contain at" << endl;
        return NULL;
    }
    root->val = "=";
    TreeNode* X = root->left->left;
    TreeNode* E = X->right;

    TreeNode* gam = new TreeNode();
    gam->val = "gamma";

    TreeNode* Ystar = new TreeNode();
    Ystar->val = "<Y*>";

    TreeNode* lam = new TreeNode();
    lam->val = "lambda";

    TreeNode* newX = new TreeNode(); //TODO-Doubt deep copy??
    newX->val = X->val;

    root->left = X;
    X->right = gam;
    gam->left = Ystar;
    Ystar->right = lam;
    lam->left = newX;
    newX->right = E;

    //TODO-Doubt delete old eq ??
    return root;
}

class Term;
class Delta;

class Environment{
public:
    Environment(){
        bounded_variable_map = new unordered_map<Term*, Term*>();
    }
    int id;
    Environment* parent;
    unordered_map<Term*, Term*>* bounded_variable_map;
    string toString();
};

string Environment::toString() {
    string str = "";
    str += "id: " + to_string(id) + "\n";
    /*for(auto kv : bounded_varible_map){
        str += (string) kv.first + " : " + to_string(kv.second);
        str += "\n";
    }*/
    return str;
}

class Term{
public:
    string type;                // gamma, lambda, cond, tau, binary_op, unary_op
    int tau_children;           // tau_children
    int value;                  // for numbers
    int lam_k;                  // k -> lambda
    int lam_c;                  // c
    vector<string>* boundedvars; // lambda
    int delta_idx;              // for conditional
    vector<Term*> tuple_children; //for TUPLE

    bool isLambda();
    bool isGamma();
    bool isTau();
    bool isTauChild();
    bool isDelta();
    bool isEnv();
    bool isNumber();
    bool isName();
    bool isRator();
    bool isRand();
    bool isBinaryOp();
    bool isUnaryOp();
    bool isPrimitiveFunc();
    bool isIdentifier();
    bool isString();
    string toString();
    int getNumber();
    string getString();
    string getPrimitiveFunc();
    bool isBool();
    bool isTuple();
    bool isYSTAR();
    bool isETA();


    bool is_num(string&);
};

bool Term::isETA() {
    return type.compare("ETA")==0;
}

bool Term::isYSTAR() {
    return type.compare("<Y*>")==0;
}

bool Term::isBool(){
    return type.compare("true")==0 || type.compare("false")==0;
}

bool Term::isTuple() {
    return type.compare("TUPLE")==0;
}

string Term::getString() {
    if(isString()){
        return type.substr(5, type.length()-6);
    }
}

string Term::getPrimitiveFunc() {
    return type.substr(4,type.length()-5);
}

int Term::getNumber(){
    if(isNumber()){
        return stoi(type.substr(5,type.length()-6));
    }else{
        return -1;
    }
}

bool Term::isBinaryOp() {
    if(type.compare("+")==0 || type.compare("-")==0 || type.compare("*")==0 || type.compare("/")==0 || type.compare("**")==0)
        return true;
    if(type.compare("or")==0 || type.compare("&")==0 || type.compare("gr")==0 || type.compare("ge")==0 || type.compare("ls")==0 || type.compare("le")==0 || type.compare("eq")==0 || type.compare("ne")==0)
        return true;
    return false;
}

bool Term::isUnaryOp() {
    if(type.compare("not")==0 || type.compare("neg")==0)
        return true;
    return false;
}

bool Term::isPrimitiveFunc() {
    return false;
}

//TODO
bool Term::isRand() {
    return isNumber() || isIdentifier() || isString() || isBool() || isTuple();
    /*
    if(!isName() && !isUnaryOp() && !isBinaryOp() && !isPrimitiveFunc())
        return true;
    return false;*/
}

bool Term::isRator() {
    return type.find("<FN:", 0) != string::npos;
}

bool Term::is_num(string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool Term::isLambda() {
    return this->type.compare("lambda")==0;
}

bool Term::isGamma(){
    return this->type.compare("gamma")==0;
}

bool Term::isTau() {
    return this->type.compare("tau")==0;
}

bool Term::isTauChild() {
    return this->type.compare("tauchild")==0;
}

bool Term::isDelta() {
    return this->type.compare("delta")==0;
}

bool Term::isEnv() {
    return this->type.compare("env")==0;
}

bool Term::isIdentifier(){
    return type.find("<ID:", 0) != string::npos;
}

bool Term::isNumber() {

    return type.find("<INT:", 0)!=string::npos;
    /*
    if(!isLambda() && !isGamma() && !isTau() && !isTauChild() && !isDelta() && !isEnv()){
        return is_num(type);
    }else{
        return false;
    }*/
}

bool Term::isName(){
    //return !isLambda() &&!isGamma() && !isTau() && !isTauChild() && !isDelta() && !isEnv() && !isNumber();
    return isIdentifier();
}

bool Term::isString(){
    return type.find("<STR:", 0)!=string::npos;
}

string Term::toString() {
    string str = "";
        //if(this==NULL) return NULL;
        if(isLambda()){
            if(boundedvars == NULL){
                return "ERROR in Term.toString()";
            }
            int vars = boundedvars->size();
            string varstr = "";
            for(int i=0; i<vars; i++)
                varstr += boundedvars->at(i) + " ";
            return "<lambda k:" + to_string(lam_k) + " env: " + to_string(value) + " vars: " + varstr + ">";
        }else if(isTau()) {
            return "<tau " + to_string(tau_children) + ">";
        }else if(isTauChild()){
            return "<tauchild " + to_string(delta_idx) + ">";
        }else if(isDelta()){
            return "<delta " + to_string(delta_idx) + ">";
        }else if(isEnv()){
            return "<env " + to_string(value) + ">";
        }
        else{
            return this->type + "";
        }
    return str;
}

class Delta{
public:
    int id;
    //Environment* env;
    vector<Term*>* terms;

    Delta(){
        terms = new vector<Term*>();
        //env = new Environment();
    }

    string toString();
};

string Delta::toString(){
    string str = "";
    if(this->terms==NULL) {
        str += "Delta is NULL or no terms in Delta";
        return str;
    }
    int ts = terms->size();
    for(int i=0; i<ts; i++)
        str += terms->at(i)->toString() + " ";
    return str;
}

class ControlStructure{
    int d;
    int k;
    int maxd;
public:
    unordered_map<int, Delta*> control_structures;
    void generate(TreeNode*);
    void generateDelta(TreeNode*, Delta*, queue<TreeNode*>*);
    void print();
};

void ControlStructure::generate(TreeNode* root) {

    if(root==NULL){
        if(debug) cout << "root is NULL cannot generate control structures" << endl;
    }

    queue<TreeNode*>* st_roots = new queue<TreeNode*>();
    st_roots->push(root);

    d = 0;
    k = 1;

    while(!st_roots->empty()){
        TreeNode* front = st_roots->front();
        st_roots->pop();
        Delta* delta = new Delta();
        generateDelta(front, delta, st_roots);
        control_structures.insert(make_pair(d, delta));
        d++;
    }

    //cout << "DONE building control structures" << endl;
}

//pre-order traversal
void ControlStructure::generateDelta(TreeNode* root, Delta* delta, queue<TreeNode*>* st_roots){

    //process root
    if(root->val.compare("lambda")==0){

        if(root->left==NULL || root->left->right==NULL){
            if(debug) cout << "child NULL for lambda to generate control structure" << endl;
            return;
        }

        Term* lam = new Term();
        lam->type = "lambda";
        lam->lam_k = k;
        lam->boundedvars = new vector<string>();

        if(root->left->val.compare(",")==0){
            //TODO
            TreeNode* curr_var = root->left->left;
            while(curr_var!=NULL){
                lam->boundedvars->push_back(curr_var->val);
                curr_var = curr_var->right;
            }
        }else{
            lam->boundedvars->push_back(root->left->val);
        }

        delta->terms->push_back(lam);
        k++;
        st_roots->push(root->left->right);
        return;

    }else if(root->val.compare("->")==0){
            //TODO
        Delta* del_then = new Delta();
        Delta* del_else = new Delta();
        Delta* beta = new Delta();

        Term* del_then_term = new Term();
        del_then_term->type = "delta";
        del_then_term->delta_idx = d+1;
        st_roots->push(root->left->right);
        //generateDelta(root->left->right, del_then, st_roots);

        Term* del_else_term = new Term();
        del_else_term->type = "delta";
        del_else_term->delta_idx = d+2;
        st_roots->push(root->left->right->right);
        //generateDelta(root->left->right->right, del_else, st_roots);

        Term* beta_term = new Term();
        beta_term->type = "beta";

        delta->terms->push_back(del_then_term);
        delta->terms->push_back(del_else_term);
        delta->terms->push_back(beta_term);
        Term* B = new Term();
        B->type = root->left->val;
        delta->terms->push_back(B);
        root = root->left;

    }else if(root->val.compare("tau")==0){
            //TODO
            Term* tau_n = new Term();
            tau_n->type = "tau";

            delta->terms->push_back(tau_n);

            TreeNode* curr_var = root->left;
            int childcount = 0;
            int idx = 1;
            while(curr_var!=NULL){
                Term* temp = new Term();
                temp->type = "tauchild";
                temp->delta_idx = d + st_roots->size() + 1; //TODO check
                st_roots->push(curr_var);
                curr_var = curr_var->right;
                idx++;
                delta->terms->push_back(temp);
                childcount++;
            }

            tau_n->tau_children = childcount;
            return;

    }else{

        Term* term = new Term();
        term->type = root->val;
        delta->terms->push_back(term);
    }

    if(root->left!=NULL)
        generateDelta(root->left, delta, st_roots);

    if(root->left!=NULL && root->left->right!=NULL)
        generateDelta(root->left->right, delta, st_roots);
}


void ControlStructure::print() {

    for(int dd=0; dd<d; dd++){
        Delta* delta = control_structures.at(dd);
        if(delta == NULL){
            cout << "NULL delta at " << dd << endl;
            continue;
        }
        cout << dd << " : " << delta->toString() << endl;
    }

}

class CSEMachine;

class CSEMachine{

public:

    CSEMachine(ControlStructure* cstr){
        if(cstr==NULL || cstr->control_structures.size()==0){
            if(debug) cout << "ERR in CSEMachine contructor!! - control sturctures could be NULL" << endl;
            return;
        }
        this->controlStructure = cstr;
        this->control = new vector<Term*>();
        this->stack1 = new stack<Term*>();
        this->appliedRules = new vector<int>();
        this->maxdelta = cstr->control_structures.size()-1;
    }

    void initialstate();
    void rule1();
    void rule2();
    void rule3();
    void rule4();
    void rule5();
    void rule6();
    void rule7();
    void rule8();
    void rule9();
    void rule10();
    void rule12();
    void rule13();
    void rec_rule();

    void execute();
    void printState();

    unordered_map<int, Environment*> env_map;

private:
    ControlStructure* controlStructure;
    int envs;
    int maxdelta;

    vector<Term*>* control;
    stack<Term*>* stack1;
    Environment* env0;
    Environment* env_curr;
    vector<int>*  appliedRules;

    Term* lookup(Term*);



    //utils
    void addDeltaToControl(Delta*);
    Term* getValueFromEnv(Term*);

    int fetchRule();
    void executeRule(int);

    void properprint(string);

    Term* apply(Term*, Term*);
    Term* apply_binary(Term*, Term*, Term*);
    Term* apply_unary(Term*, Term*);

};

void CSEMachine::properprint(string s){
    for ( int i = 0 ; i < s.length() ; i++ )
    {
        if( s[i] == '\n')
            cout<<"\\n";
        else
            cout << s[i];
    }
}

Term* CSEMachine::apply(Term* rator, Term* rand){
    string pfunc = rator->getPrimitiveFunc();
    if(pfunc.compare("Print")==0){
        Term* dummy = new Term();
        if(rand->isNumber()){
            if(output) cout << rand->getNumber() ;
        }else if(rand->isString()){
            if(output) properprint(rand->getString()) ;
        }
        dummy->type = "dummy";
        return dummy;
    }
    return NULL;
}

Term* CSEMachine::apply_binary(Term* binOp, Term* rand1, Term* rand2) {
    int val1 = rand1->getNumber();
    int val2 = rand2->getNumber();
    Term* res = new Term();
    if(binOp->type.compare("+")==0){
        res->type = "<INT:" + to_string(val1 + val2) + ">";
        return res;
    }else if(binOp->type.compare("-")==0){
        res->type = "<INT:" + to_string(val1 - val2) + ">";
        return res;
    }else if(binOp->type.compare("*")==0){
        res->type = "<INT:" + to_string(val1*val2) + ">";
        return res;
    }else if(binOp->type.compare("/")==0){
        res->type = "<INT:" + to_string(val1/val2) + ">";
        return res;
    }
    return NULL;
}

Term* CSEMachine::apply_unary(Term* unop, Term* rand) {
    /*if(rand->isNumber()){

    }*/
    return NULL;
}

int CSEMachine::fetchRule() {

    if(control->size()==0 && stack1->size()==1){
        Term* res = stack1->top();
        if(output && res->type.compare("dummy")!=0)
            cout << res->type << endl;
        return -5;
    }

    Term* ctrl_last = control->at(control->size()-1);

    if(ctrl_last->isYSTAR()){
        control->pop_back();
        stack1->push(ctrl_last);
        return fetchRule();
    }

    if(ctrl_last->type.compare("delta")==0){
        control->pop_back();
        addDeltaToControl(controlStructure->control_structures.at(ctrl_last->delta_idx));
        if(debug) cout << "Loading Delta" << endl << endl;
        if(debug) printState();
        return fetchRule();
    }

    if(ctrl_last->type.compare("tauchild")==0){
        control->pop_back();
        addDeltaToControl(controlStructure->control_structures.at(ctrl_last->delta_idx));
        if(debug) cout << "Loading tauchild" << endl;
        if(debug) printState();
        return fetchRule();
    }

    //if(!ctrl_last->isBinaryOp() && !ctrl_last->isUnaryOp() && (ctrl_last->isName() || ctrl_last->isNumber() || ctrl_last->isString())){
    if(ctrl_last->isRand()){
        //Rule 1
        return 1;
    }else if(ctrl_last->isLambda()){
        //Rule 2
        return 2;
    }else if(ctrl_last->isEnv()){
        //Rule 5
        Term* v = stack1->top();
        //TODO check (env     lambda env) also possible !!
        if(v->isRand() || v->type.compare("value")==0 || v->is_num(v->type) || v->isLambda() || v->type.compare("dummy")==0){
            stack1->pop();
            Term* e = stack1->top();
            if(e->type.compare("env")==0){
                stack1->push(v);
                return 5;
            }else{
                stack1->push(v);
                if(debug) cout << "ERR in fetchRule() - while checking for Rule 5" << endl;
                return -1;
            }
        }
    }

    if(ctrl_last->isGamma()){

        //Rule 3
        Term* rtr = stack1->top();
        if(rtr->isRator()){
            stack1->pop();
            Term* rnd = stack1->top();
            if(rnd->isRand()){
                stack1->push(rtr);
                return 3;
            }else{
                stack1->push(rtr);
                if(debug) cout << "ERR in fetchRule() - while checking for Rule 3" << endl;
                return -1;
            }
        }

        //Rule 4
        Term* lam = stack1->top();
        if(lam->isLambda()){
            stack1->pop();
            Term* rnd = stack1->top();
            if(rnd->isETA()){
                stack1->push(lam);
                if(debug) cout << "SPECIAL REC RULE" << endl;
                return 15;
            }
            if(rnd->isRand() || rnd->isLambda() || rnd->isTuple()){ //TODO check Special case -- see tests/add
                stack1->push(lam);
                return 4;
            }else{
                stack1->push(lam);
                if(debug) cout << "ERR in fetchRule() - while checking for Rule 4" << endl;
                return -1;
            }
        }

        //Rule10
        Term* tuple = stack1->top();
        if(tuple->isTuple()){
            stack1->pop();
            Term* idx = stack1->top();
            if(idx->isNumber()){
                stack1->push(tuple);
                return 10;
            }else{
                stack1->push(tuple);
                if(debug) cout << "ERR in fetchRule() - while checking for Rule 10" << endl;
                return -1;
            }
        }

        //Rule12
        Term* ystar = stack1->top();
        if(ystar->isYSTAR()){
            stack1->pop();
            Term* lm = stack1->top();
            if(lm->isLambda()){
                stack1->push(ystar);
                return 12;
            }else{
                stack1->push(ystar);
                if(debug) cout << "ERR in fetchRule() - while checking for Rule 12" << endl;
                return -1;
            }
        }

        //Rule13
        Term* eta = stack1->top();
        if(eta->isETA()){
            return 13;
        }

    }

    //Rule 6
    if(ctrl_last->isBinaryOp()){
        Term* rnd1 = stack1->top();
        if(rnd1->isRand()){
            stack1->pop();
            Term* rnd2 = stack1->top();
            if(rnd2->isRand()) {
                stack1->push(rnd1);
                return 6;
            }else{
                stack1->push(rnd1);
                if(debug) cout << "ERR in fetchRule() - while checking for Rule 6" << endl;
                return -1;
            }
        }else{
            if(debug) cout << "ERR in fetchRule() - while checking for Rule 6" << endl;
            return -1;
        }
    }

    //Rule 7
    if(ctrl_last->isUnaryOp()){
        Term* rand = stack1->top();
        if(rand->isRand()){
            return 7;
        }
        if(debug) cout << "ERR in fetchRule() - while checking for Rule 7" << endl;
        return -1;
    }

    //Rule8
    if(ctrl_last->type.compare("beta")==0){
        Term* t = stack1->top();
        if(t->type.compare("true")==0){
            return 8;
        }else if(t->type.compare("false")==0){
            return 8;
        }else{
            if(debug) cout << "ERR in fetchRule() - while checking for Rule 8" << endl;
            return -1;
        }
    }

    //Rule9
    if(ctrl_last->type.compare("tau")==0){
        int tauchildren = ctrl_last->tau_children;
        stack<Term*>* temp = new stack<Term*>();
        for(int i=0; i<tauchildren; i++){
            Term* v = (Term*) stack1->top();
            if(!v->isRand()){
                if(debug) cout << "ERR in fetchRule() - while checking for Rule 9" << endl;
                return -1;
            }
            temp->push(v);
            stack1->pop();
        }
        while(!temp->empty()){
            Term* v = (Term*) temp->top();
            stack1->push(v);
            temp->pop();
        }
        return 9;
    }


    if(debug) cout << "end of rules!!" << endl;
    return -1;

}

void CSEMachine::execute() {
    int rule = 0;
    while((rule = fetchRule())>0){
        if(debug) cout << " Executing Rule: " + to_string(rule) << endl << endl;
        executeRule(rule);
        appliedRules->push_back(rule);
        if(debug) printState();
    }
}

void CSEMachine::executeRule(int rule) {
   switch(rule){
       case 1:  rule1();
                break;
       case 2:  rule2();
                break;
       case 3:  rule3();
                break;
       case 4:  rule4();
                break;
       case 5:  rule5();
                break;
       case 6:  rule6();
                break;
       case 7:  rule7();
                break;
       case 8:  rule8();
                break;
       case 9:  rule9();
                break;
       case 10: rule10();
                break;
       case 12: rule12();
                break;
       case 13: rule13();
                break;
       case 15: rec_rule();
                break;
       default: break;
   }
}

Term* CSEMachine::getValueFromEnv(Term* name) {
    Environment* tmp = env_curr;
    while(tmp!=NULL){

        for(auto it = tmp->bounded_variable_map->begin(); it!=tmp->bounded_variable_map->end(); ++it){
            Term* x = it->first;
            Term* val = it->second;
            if(x->type.compare(name->type)==0){
                Term* res = val;
                res->type = val->type;
                return res;
            }
        }
        tmp = tmp->parent;
    }


    string pfunc = name->getPrimitiveFunc();
    Term* f = new Term();
    f->type = "<FN:" + pfunc + ">";
    return f;
}

void CSEMachine::addDeltaToControl(Delta * delta) {
    if(delta == NULL){
        if(debug) cout << "ERR in addDeltaToControl() - delta is NULL" << endl;
        return;
    }
    for(int i=0; i<delta->terms->size(); i++){
        control->push_back(delta->terms->at(i));
    }
}

void CSEMachine::printState() {
    if(appliedRules==NULL || appliedRules->size()==0){
        //cout << "initialState:" << endl;
    }else{
        //cout << "on step: " << appliedRules->size() + 1 << endl;
    }

    //cout << "CONTROL: size = " << control->size() << endl;
    cout << "CONTROL: " ;
    for(vector<Term*>::iterator it = control->begin(); it != control->end(); ++it){
        Term* t = *it;
        cout << t->toString() << " " ;
    }
    cout << endl;

    cout << "STACK: " ;
    stack<Term*>* temp = new stack<Term*>();
    while(!stack1->empty()){
        Term* t = (Term*) stack1->top();
        cout << t->toString() << " ";
        temp->push(t);
        stack1->pop();
    }
    while(!temp->empty()){
        Term* t = (Term*) temp->top();
        stack1->push(t);
        temp->pop();
    }

    cout << endl <<  "ENVIRONMENT: ";
    cout << env_curr->toString();

}

void CSEMachine::initialstate() {
    Term* ctrl_e0 = new Term();
    ctrl_e0->type = "env";
    ctrl_e0->value = 0;
    control->push_back(ctrl_e0);
    Delta* del0 = controlStructure->control_structures.at(0);
    addDeltaToControl(del0);

    Term* stk_e0 = new Term();
    stk_e0->type = "env";
    stk_e0->value = 0;
    stack1->push(stk_e0);

    env0 = new Environment();
    env0->id = 0;
    env_map.insert(make_pair(0, env0));
    envs = 1;
    env_curr = env0;
    env0->parent = NULL;
    //TODO - initialize env0 to PrimitiveEnvironment

}

Term* CSEMachine::lookup(Term* ctrl_term) {
    if(ctrl_term->isNumber() || ctrl_term->isString()){
        return ctrl_term;
    }
    if(ctrl_term->isIdentifier() || ctrl_term->isTuple() || ctrl_term->isLambda()){
        return getValueFromEnv(ctrl_term);
    }else{
        if(debug) cout << "ERROR: Term is not NAME - cannot do lookup" << endl;
        return NULL;
    }
}


/**
 *  if(Name in Control)
 *      Stack.push(lookup(Name))
 */
void CSEMachine::rule1() {
    Term* name = control->back();
    control->pop_back();
    Term* ob = lookup(name);
    stack1->push(ob);
}

void CSEMachine::rule2() {
    Term* lam = control->back();
    control->pop_back();
    lam->value = env_curr->id;
    stack1->push(lam);
}

void CSEMachine::rule3(){
    Term* gam = control->back();
    control->pop_back();
    Term* rtr = stack1->top();
    stack1->pop();
    Term* rnd = stack1->top();
    stack1->pop();
    Term* res = apply(rtr, rnd);
    stack1->push(res);
}

//Rule 4 and Rule 11
void CSEMachine::rule4(){
    Term* gam = control->back();
    control->pop_back();
    Term* lam = stack1->top();
    stack1->pop();

    Environment* env_n = new Environment();
    env_n->id = envs;
    env_n->parent = env_map.at(lam->value);
    env_map.insert(make_pair(envs, env_n));
    envs++;

    Term* ctrl_env = new Term();
    ctrl_env->type = "env";
    ctrl_env->value = env_n->id;
    control->push_back(ctrl_env);

    Delta* delta = controlStructure->control_structures.at(lam->lam_k);
    Term* ctrl_del = new Term();
    ctrl_del->type ="delta";
    ctrl_del->delta_idx = lam->lam_k;
    control->push_back(ctrl_del);

    //Term* rand = stack1->top();
    //stack1->pop();

    //Term* x = new Term();
    //x->type = *lam->boundedvars->begin();
    //make_pair(x, rand);
    //env_n->bounded_variable_map->insert(make_pair(x, rand));

    int varsize = lam->boundedvars->size();

    Term* top = stack1->top();
    bool boundedToTuple = false;
    if(varsize > 1 && top->isTuple()){ //TODO check - Assuming lambda bounded vars is equal to the next tuple size
        if(top->tuple_children.size()!=varsize){
            if(debug) cout << "in rule4() -- Wrong Assumption that lambda bounded vars = next tuple size" << endl;
            return;
        }
        for(int i=0; i<varsize; i++){
            Term* x = new Term();
            x->type = lam->boundedvars->at(i);
            Term* tup_val = top->tuple_children.at(i);
            env_n->bounded_variable_map->insert(make_pair(x, tup_val));
        }
        boundedToTuple = true;
        stack1->pop(); //pop tuple
    }


    for(int i=0; i<varsize && !boundedToTuple; i++){
        Term* x = new Term();
        x->type = lam->boundedvars->at(i);
        Term* rand = stack1->top();
        stack1->pop();
        env_n->bounded_variable_map->insert(make_pair(x, rand));
    }


    Term* stck_env = new Term();
    stck_env->type = "env";
    stck_env->value = env_n->id;
    stack1->push(stck_env);

    env_curr = env_n;
}

void CSEMachine::rule5(){
    Term* env_n = control->back();
    control->pop_back();
    Term* value = stack1->top();
    stack1->pop();
    Term* stack_env_n = stack1->top();
    stack1->pop();
    stack1->push(value);
}

void CSEMachine::rule6(){
    //TODO
    Term* bop = control->back();
    control->pop_back();

    Term* rand1 = stack1->top();
    stack1->pop();
    Term* rand2 = stack1->top();
    stack1->pop();

    Term* res = apply_binary(bop, rand1, rand2);
    stack1->push(res);
}

void CSEMachine::rule7(){
    Term* uop = control->back();
    control->pop_back();
    Term* rand = stack1->top();
    stack1->pop();
    Term* res = apply_unary(uop, rand);
    stack1->push(res);
}

void CSEMachine::rule8() {
    Term* beta = control->back();
    control->pop_back();
    Term* boo = stack1->top();
    if(boo->type.compare("true")==0){
        control->pop_back();
    }else if(boo->type.compare("false")==0){
        Term* del_else = control->back();
        control->pop_back();
        control->pop_back();
        control->push_back(del_else);
    }
}

void CSEMachine::rule9(){
    Term* taun = control->back();
    control->pop_back();
    int tau_children = taun->tau_children;
    Term* tuple = new Term();
    tuple->type = "TUPLE";
    for(int i=0; i<tau_children; i++){
        Term* v = stack1->top();
        tuple->tuple_children.push_back(v);
        stack1->pop();
    }
    stack1->push(tuple);
}

void CSEMachine::rule10(){
    control->pop_back(); //pop gamma
    Term* tuple = stack1->top();
    stack1->pop();
    Term* idx = stack1->top();
    stack1->pop();
    int index = idx->getNumber();
    Term* vi = tuple->tuple_children.at(index);
    stack1->push(vi);
}

void CSEMachine::rule12(){
    //TODO
    control->pop_back(); //pop gamma
    stack1->pop(); //pop ystar
    Term* lam = stack1->top();
    stack1->pop();
    Term* eta = new Term();
    eta->type = "ETA";
    eta->tuple_children.push_back(lam);
    stack1->push(eta);

}

void CSEMachine::rule13(){
    Term* gam = new Term();
    gam->type = "gamma";
    control->push_back(gam);
    Term* eta = stack1->top();
    Term* lam = eta->tuple_children.at(0);
    stack1->push(lam);
}


void CSEMachine::rec_rule() {
    Term* gam = control->back();
    Term* lam = stack1->top();
    stack1->pop();
    Term* eta = stack1->top();
    stack1->pop(); //pop eta

    Environment* env_n = new Environment();
    env_n->id = envs;
    env_n->parent = env_map.at(lam->value);
    env_map.insert(make_pair(envs, env_n));
    envs++;

    Term* ctrl_env = new Term();
    ctrl_env->type = "env";
    ctrl_env->value = env_n->id;
    control->push_back(ctrl_env);

    Delta* delta = controlStructure->control_structures.at(lam->lam_k);
    addDeltaToControl(delta);

    Term* stck_env = new Term();
    stck_env->type = "env";
    stck_env->value = env_n->id;
    stack1->push(stck_env);

    env_curr = env_n;

}

/**
 * standardize AST ends here
 */

/**
*    main START
*/

int main(int argc, char* argv[]) {
    string inputfile;
    bool printAST = false;
    bool printST = false;

    if(argc < 2){
        exit(0);
    }

    if(argc == 2) {
        inputfile = argv[1];
        output = true;
    }else if(argc == 3){
        inputfile = argv[2];
        if(((string) argv[1]).compare("-ast")==0)
            printAST = true;
        if(((string) argv[1]).compare("-st")==0)
            printST = true;
    }else{
        //cout << "ERROR: More args than expected!" << endl;
        exit(0);
    }

    Parser parser(inputfile);
    parser.E();

    if(printAST)
        parser.preorderTraversal(parser.top(), 0);

    parser.standardize_AST(parser.top());

    if(printST)
        parser.preorderTraversal(parser.top(), 0);

    // working code!!
    ControlStructure* controlStructure = new ControlStructure();
    controlStructure->generate(parser.top());

    CSEMachine* cseMachine = new CSEMachine(controlStructure);
    cseMachine->initialstate();
    cseMachine->execute();

    return 0;
}
