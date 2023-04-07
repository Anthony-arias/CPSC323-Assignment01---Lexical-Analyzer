
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
    void optDeclarationList();
    void statementList();
    // Rule 3 & 4
    void functionDefinitions();
    void function();
    void functionDefinitionsPrime();
    // Rule 5
    void identifier();
    void optParameterList();
    void body();
    // Rule 6
    void parameterList();
    // Rule 7
    void parameter();
    void parameterListPrime();
    // Rule 9
    void ids();
    void qualifier();
    // Rule 12
    void declarationList();

    //Rule 12-17
    void declarationListPrime();
    void declaration();
    void idsPrime();

    //Rule 18 <Statement  List>::= <Statement><Statement List'>
    void statementListPrime();
    void statement();

    //Rule 20 <Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
    void compound();
    void assign();
    void ifFunction();
    void ifFunctionPrime();

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

    // Debug terminal hardcoded ON to output parse tree
    bool terminal = true;

    LexerAnalyzer lexer;
    int current_token_index;
};

