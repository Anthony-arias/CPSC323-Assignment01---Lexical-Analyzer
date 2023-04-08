
#pragma once
#include "LexerAnalyzer.h"

class SyntaxAnalyzer {
public:

    vector<Token> syntaxTokens;

    //helper functions
    void outputTokenValueAndIterate();
    void throwError();

    // Rule 1
    void rat23S();
    // Rule 2
    void optFunctionDefinitions();
    //void optDeclarationList();
    //void statementList();
    void functionDefinitions();
    void function();
    void functionDefinitionsPrime();
    void optParameterList();
    void parameterList();
    void parameterListPrime();
    void parameter();
    void qualifier();
    void body();
    void optDeclarationList();
    void declarationList();
    void declarationListPrime();
    void declaration();
    void ids();
    void idsPrime();
    void statementList();
    void statementListPrime();
    void statement();
    void compound();
    void assign();
    void ifRule();
    void ifRulePrime();
   

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

