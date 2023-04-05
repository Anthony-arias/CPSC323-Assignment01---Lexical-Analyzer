
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
    bool functionDefinitions();
    bool function();
    bool functionDefinitionsPrime();
    bool qualifier();

    //Rule 13-24
    bool declarationList();
    bool declarationListPrime();
    bool declaration();
    bool ids();
    bool idsPrime();

private:
    LexerAnalyzer lexer;
    int current_token_index;
};

