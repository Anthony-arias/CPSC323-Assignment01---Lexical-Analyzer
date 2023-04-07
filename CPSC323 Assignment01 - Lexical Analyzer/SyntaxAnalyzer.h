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
    bool qualifier();

    //Rule 13-24
    bool declarationList();
    bool declarationListPrime();
    bool declaration();
    bool ids();
    bool idsPrime();

private:

    // Debug terminal hardcoded ON to output parse tree
    bool terminal = true;

    LexerAnalyzer lexer;
    int current_token_index;
};
