
#pragma once
#include "LexerAnalyzer.h"

class SyntaxAnalyzer {
public:

    vector<Token> syntaxTokens;

    // Rule 1
    bool rat23S();
    // Rule 2
    bool optFunctionDefinitions();
    bool optDeclarationList();
    bool statementList();
    // Rule 3 & 4
    bool functionDefinitions();
    bool function();
    bool functionDefinitionsPrime();
    // Rule 5
    bool identifier();
    bool optParameterList();
    bool body();
    // Rule 6
    bool parameterList();
    // Rule 7
    bool parameter();
    bool parameterListPrime();
    // Rule 9
    bool ids();
    bool qualifier();
    // Rule 12
    bool declarationList();

    //abcd

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

private:

    // Debug terminal hardcoded ON to output parse tree
    bool terminal = true;

    LexerAnalyzer lexer;
    int current_token_index;
};

