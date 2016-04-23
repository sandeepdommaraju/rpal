#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <stack>

using namespace std;

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

bool debug = false;
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
    else if(root->val.compare("at")==0)
        standardize_AT(root);
    else if(root->val.compare("rec")==0)
        standardize_REC(root);
    //else
    //    cout << "ERR: found undefined node type while standardizing AST" << endl;
}

//LET
TreeNode* Parser::standardize_LET(TreeNode *root) {
    if(root==NULL || root->left==NULL || root->left->right==NULL || root->left->left==NULL || root->left->left->right==NULL){
        cout << "let subtree is not proper to standardize_LET" << endl;
        return NULL;
    }

    if(root->val.compare("let")!=0){
        cout << "let AST does not contain let" << endl;
        return NULL;
    }

    if(root->left->val.compare("=")!=0){
        cout << "let AST does not contain equals" << endl;
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
        cout << "where subtree is not proper to standardize_WHERE" << endl;
        return NULL;
    }

    if(root->val.compare("where")!=0){
        cout << "where AST does not contain where" << endl;
        return NULL;
    }

    if(root->left->right->val.compare("=")!=0){
        cout << "where does not contain equals" << endl;
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
        cout << "within subtree is not proper to standardize_WITHIN" << endl;
        return NULL;
    }

    if(root->val.compare("within")!=0){
        cout << "within AST does not contain within" << endl;
        return NULL;
    }

    if(root->left->val.compare("=")!=0 || root->left->right->val.compare("=")!=0){
        cout << "within does not contain equals" << endl;
        return NULL;
    }

    TreeNode* x1 = root->left->left;
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
    return root;

}

//FUNCFORM
TreeNode* Parser::standardize_FUNCFORM(TreeNode *root) {
    if(root==NULL || root->left==NULL || root->left->right==NULL || root->left->right->right==NULL){
        cout << "funcform subtree is not proper to standardize_FUNCFORM" << endl;
        return NULL;
    }

    if(root->val.compare("function_form")!=0){
        cout << "funcform AST does not contain function_form" << endl;
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
        cout << "and subtree is not proper to standardize_AND" << endl;
        return NULL;
    }
    if(root->val.compare("and")!=0){
        cout << "and AST does not contain and" << endl;
        return NULL;
    }
    if(root->left->val.compare("=")!=0 || root->left->right->val.compare("=")!=0){
        cout << "left/right child is not = to standardize_AND" << endl;
        return NULL;
    }

    TreeNode* eq = root->left;
    if(eq->left==NULL || eq->left->right==NULL){
        cout << "not even a single assignment/binding found to standardize_AND" << endl;
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
            cout << "one child of equal is NULL to standardize_AND" << endl;
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
        cout << "at subtree is not proper to standardize_AT" << endl;
        return NULL;
    }
    if(root->val.compare("@")!=0){
        cout << "at AST does not contain at" << endl;
        return NULL;
    }

    root->val = "gamma";
    TreeNode* E1 = root->left;
    TreeNode* N =  E1->right;
    TreeNode* E2 = N->right;

    if(E1==NULL || E2==NULL || N==NULL){
        cout << "E1/N/E2 is null to standardize_AT" << endl;
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
        cout << "rec subtree is not proper to standardize_REC" << endl;
        return NULL;
    }
    if(root->val.compare("rec")!=0){
        cout << "at AST does not contain at" << endl;
        return NULL;
    }
    root->val = "=";
    TreeNode* X = root->left->left;
    TreeNode* E = X->right;

    TreeNode* gam = new TreeNode();
    gam->val = "gamma";

    TreeNode* Ystar = new TreeNode();
    Ystar->val = "Y*";

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

    return 0;
}
