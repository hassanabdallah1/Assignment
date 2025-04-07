#include <iostream>
#include <fstream>
#include <cctype>
#include <string>

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

class Lexer {
private:
    /* Variables */
    int charClass;
    std::string lexeme;
    char nextChar;
    int nextToken;
    std::ifstream in_fp;

    /* Function declarations */
    void addChar();
    void getChar();
    void getNonBlank();
    int lookup(char ch);

public:
    Lexer() : charClass(0), nextChar(0), nextToken(0) {}

    /* Main lexical analysis function */
    int lex();

    /* Open input file */
    bool openFile(const std::string& filename);

    /* Run lexical analysis */
    void analyze();
};

/* Open input file */
bool Lexer::openFile(const std::string& filename) {
    in_fp.open(filename);
    if (!in_fp) {
        std::cout << "ERROR - cannot open " << filename << std::endl;
        return false;
    }
    return true;
}

/******************************************************/
/* lookup - a function to lookup operators and parentheses
and return the token */
int Lexer::lookup(char ch) {
    lexeme += ch;
    switch (ch) {
    case '(': nextToken = LEFT_PAREN; break;
    case ')': nextToken = RIGHT_PAREN; break;
    case '+': nextToken = ADD_OP; break;
    case '-': nextToken = SUB_OP; break;
    case '*': nextToken = MULT_OP; break;
    case '/': nextToken = DIV_OP; break;
    case '=': nextToken = ASSIGN_OP; break;
    default:
        nextToken = EOF;
        break;
    }
    return nextToken;
}

/*****************************************************/
/* getChar - a function to get the next character of
input and determine its character class */
void Lexer::getChar() {
    if (in_fp.get(nextChar)) {
        if (std::isalpha(nextChar))
            charClass = LETTER;
        else if (std::isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    }
    else
        charClass = EOF;
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void Lexer::getNonBlank() {
    while (std::isspace(nextChar))
        getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic
expressions */
int Lexer::lex() {
    lexeme.clear();
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
    case LETTER:
        while (charClass == LETTER || charClass == DIGIT) {
            lexeme += nextChar;
            getChar();
        }
        nextToken = IDENT;
        break;

        /* Parse integer literals */
    case DIGIT:
        while (charClass == DIGIT) {
            lexeme += nextChar;
            getChar();
        }
        nextToken = INT_LIT;
        break;

        /* Parentheses and operators */
    case UNKNOWN:
        lookup(nextChar);
        getChar();
        break;

        /* EOF */
    case EOF:
        nextToken = EOF;
        lexeme = "EOF";
        break;
    }


    // Print token and lexeme
    std::cout << "Next token is: " << nextToken
        << ", Next lexeme is " << lexeme << std::endl;

    return nextToken;
}

/* Run lexical analysis */
void Lexer::analyze() {
    getChar();
    do {
        lex();
    } while (nextToken != EOF);
}

/******************************************************/
/* main driver */
int main() {
    Lexer lexer;

    // Open the input data file
    if (!lexer.openFile("front.in")) {
        return 1;
    }

    // Perform lexical analysis
    lexer.analyze();

    return 0;
}