
#pragma once
#include "LexerAnalyzer.h"

class SyntaxAnalyzer {
public:

    vector<Token> syntaxTokens;
    //helper functions
    void outputTokenValueAndIterate();
    void throwError();

    // Rule 1
    bool rat23S();
    // Rule 2
    bool optFunctionDefinitions();
    bool optDeclarationList();
    bool statementList();
    bool functionDefinitions();
    bool function();
    bool functionDefinitionsPrime();













    // Rule 25 - 37
    void returnRule();
    void returnRulePrime();
    void print();
    void scan();
    void whileRule();
    void condition();
    void relop();
    void expression();
    void expressionPrime();
    void term();
    void termPrime();
    void factor();
    void primary();
private:
    LexerAnalyzer lexer;
    int current_token_index;
};

