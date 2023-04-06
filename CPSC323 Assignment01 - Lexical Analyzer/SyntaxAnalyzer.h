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

private:

    // Debug terminal hardcoded ON to output parse tree
    bool terminal = true;

    LexerAnalyzer lexer;
    int current_token_index;
};
