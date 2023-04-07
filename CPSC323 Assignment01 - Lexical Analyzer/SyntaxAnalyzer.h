
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
    bool declarationListPrime();
    bool declaration();
    bool idsPrime();

    //Rule 18 <Statement  List>::= <Statement><Statement List'>
    bool statementListPrime();
    bool statement();

    //Rule 20 <Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
    bool compound();
    bool assign();
    bool ifFunction();
    bool ifFunctionPrime();
    bool returnFunction();
    bool print();
    bool scan();
    bool whileFunction();
    bool expression();

    //Rule 30
    bool condition();













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

