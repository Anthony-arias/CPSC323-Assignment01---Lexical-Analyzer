#pragma once
#include "LexerAnalyzer.h"

class SyntaxAnalyzer {
public:

    vector<Token> syntaxTokens;

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

private:

    // Debug terminal hardcoded ON to output parse tree
    bool terminal = true;

    LexerAnalyzer lexer;
    int current_token_index;
};
