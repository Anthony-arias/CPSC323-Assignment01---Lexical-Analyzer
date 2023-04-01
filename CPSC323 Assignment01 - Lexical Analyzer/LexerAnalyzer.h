#pragma once
#include "Global_Includes.h"

struct Token 
{
    string type;
    string value;
};

class LexerAnalyzer
{
public:

    vector<Token> lexer(string input);


private:

    // Helper functions

    bool checkIfSep(char c);
    string checkIfOp(string input, int& i, bool& isOp);
    int checkIfComment(char c, string input, int index);
    bool checkIfKeyword(string lexeme);
    Token checkStates(int state, Token token, string lexeme);

    // Variables

    // DFSM table values
    vector<vector<int>> dfsm_table = {
        //   L  D  _  .
            {1, 2, 3, 3}, // state 0 START
            {1, 5, 6, 3}, // state 1 ID
            {3, 2, 3, 4}, // state 2 INT
            {3, 3, 3, 3}, // state 3 ERROR
            {3, 7, 3, 3}, // state 4 ERROR
            {1, 5, 6, 3}, // state 5 ID
            {1, 5, 6, 3}, // state 6 ID
            {3, 7, 3, 3}  // state 7 REAL
    };

    vector<int> accepting_states = { 1, 2, 5, 6, 7 };

    vector<string> keywords = { "function", "int", "bool", "real","if", "fi", "else", "return", "put", "get", "while","endwhile", "true", "false" };

    vector<string> ops = { "==", "!=", ">", "<", "<=", "=>", "+", "-", "*", "/", "=" };

    vector<char> seps = { '#', '(', ')',',','{','}',';' };
};


