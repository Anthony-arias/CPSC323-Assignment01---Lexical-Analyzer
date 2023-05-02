
#pragma once
#include "LexerAnalyzer.h"

class SyntaxAnalyzer {
public:

    ofstream file;
    vector<Token> syntaxTokens;
    string outputString;

    SymbolTable symTable;

    // 1st: instr address, 2nd: operator, 3rd: operand
    string Instr_table[1000][3];

    vector<int> jump_stack;
    int instr_address = 1;
    string save;

    //helper functions
    void outputTokenValueAndIterate();
    void throwError(string expectedTokenType, string expectedToken);
    string toUpper(std::string str);
    void fileOpen(string input);
    void gen_instr(string op, string oprnd);
    int get_address(string token);
    void print_Instr_table(string instr_table[][3], int table_size);
    void back_patch(int jump_addr);
    int pop_jumpstack();
    void push_jumpstack(int jump_addr);
    void writeTableToFile(string output_file) const;

    // Rule 1
    void rat23S();
    // Rule 2
    void optFunctionDefinitions();
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
    bool printRules = false;
    LexerAnalyzer lexer;
    int current_token_index = 0;
    
};

