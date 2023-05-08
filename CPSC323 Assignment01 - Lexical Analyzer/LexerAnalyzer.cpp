#include "LexerAnalyzer.h"

// Checks if char is a separtor
bool LexerAnalyzer::checkIfSep(char c)
{
    // Loop through entire separtor vector to see if the char is a separtor
    if (find(seps.begin(), seps.end(), c) != seps.end()) {
        return true;
    }
    else {
        return false;
    }
}

// Checks to see if char is an OPERATOR 
string LexerAnalyzer::checkIfOp(string input, int& i, bool& isOp) {
    string op = "";
    char c = input[i];

    // Check if the current character is part of a two-character operator
    if (i < input.length() - 1) {
        char next = input[i + 1];
        op = string(1, c) + next;
        //if (i == 4) cout << op << endl;
        if (find(ops.begin(), ops.end(), op) != ops.end()) {
            isOp = true;
            i++;  // Skip past the second character
            return op;
        }
    }

    // Check if the current character is a one-character operator
    op = string(1, c);
    if (find(ops.begin(), ops.end(), op) != ops.end()) {
        isOp = true;
        return op;
    }

    // If the current character is not an operator, return an empty string
    isOp = false;
    return "";
}

// Checks to see a block of comment so we can ignore it
int LexerAnalyzer::checkIfComment(char c, string input, int index)
{
    // Check if this is the start of a comment
    if (c == '[' && input[index + 1] == '*')
    {
        // Search for the end of the comment
        bool found_end = false;
        int i = index + 2;
        while (i < input.length() - 1)
        {
            if (input[i] == '*' && input[i + 1] == ']')
            {
                found_end = true;
                i += 2;
                break;
            }
            i++;
        }

        // If we found end of comment return new index
        if (found_end)
        {
            return i;
        }
        else
        {
            cout << "COMMENT DOES NOT END" << endl;
        }
    }

    // If it isn't the start of a comment index remains untouched
    return index;
}

// Checks to see if IDENTIFIER is a keyword
bool LexerAnalyzer::checkIfKeyword(string lexeme)
{
    // Loop through entire keyword vector to see if the char is a keyword
    if (find(keywords.begin(), keywords.end(), lexeme) != keywords.end()) {
        return true;
    }
    else {
        return false;
    }
}

// Checks to see what accepting state the dfsm landed on to determine what token it is
Token LexerAnalyzer::checkStates(int state, Token token, string lexeme)
{
    if (state == 2) {
        token.type = "INTEGER";
    }
    else if (state == 7) {
        token.type = "REAL";
    }
    else {
        bool isKeywrd = checkIfKeyword(lexeme);
        if (isKeywrd) {
            token.type = "KEYWORD";
        }
        else {
            token.type = "IDENTIFIER";

            // If the token is an identifier, insert it into the symbol table
            if (token.type == "IDENTIFIER") {
              //  symTable.insert(lexeme, token.value);
            }
        }
    }
    token.value = lexeme;

    return token;
}

// Lexer function returns a vector of tokens (token.type, token.values) and goes one char
//      at a time reading the entire input_file that was given to separate each token and
//      value.
vector<Token> LexerAnalyzer::lexer(string input) {
    vector<Token> tokens;
    int state = 0;
    string lexeme = "";
    Token token;
    bool isSep = false;
    bool isOp = false;
    string op = "";

    for (int i = 0; i < input.length(); i++) {
        char c = input[i];

        i = checkIfComment(c, input, i);
        c = input[i];

        isSep = checkIfSep(c);
        op = checkIfOp(input, i, isOp);

        // Skip blanks, tabs, and newlines
        if (c == ' ' || c == '\t' || c == '\n' || c == '\0') {
            // Check if the current lexeme is an accepted token
            if (find(accepting_states.begin(), accepting_states.end(), state) != accepting_states.end()) {
                // Create a token for the current lexeme
                token = checkStates(state, token, lexeme);

                // Add the token to the list of tokens
                tokens.push_back(token);

                // Reset the lexeme and state for the next token
                lexeme = "";
                state = 0;
            }
            else {
                // Reset the lexeme and state for the next token
                lexeme = "";
                state = 0;
            }
        }
        // If it isn't a blank, tab, or new line
        else {
            int input_index = -1;

            // Check if its a letter
            if (isalpha(c)) {
                input_index = 0;
            }
            // Check if its a digit
            else if (isdigit(c)) {
                input_index = 1;
            }
            // Check if its an _
            else if (c == '_') {
                input_index = 2;
            }
            // Check if its a .
            else if (c == '.') {
                input_index = 3;
            }
            // If its none of those it may be a op or sep
            else {

                // Check if its a separtor
                if (isSep || isOp)
                {
                    // Check if there is a current token being worked on before adding sepator token
                    if (find(accepting_states.begin(), accepting_states.end(), state) != accepting_states.end()) {

                        // Create a token for the current lexeme
                        token = checkStates(state, token, lexeme);

                        // Add the token to the list of tokens
                        tokens.push_back(token);

                        // Reset the lexeme and state for the next token
                        lexeme = "";
                        state = 0;
                    }
                    // Add separtor token
                    if (isSep)
                    {
                        token.type = "SEPARATOR";
                        lexeme += c;
                        token.value = lexeme;
                        tokens.push_back(token);
                    }
                    else if (isOp)
                    {
                        token.type = "OPERATOR";
                        lexeme = op;
                        token.value = lexeme;
                        tokens.push_back(token);
                    }

                }
                else {

                    // Invalid input, return an error token
                    cout << endl << lexeme << endl << c << endl << static_cast<int>(c) << endl;
                    token.type = "ERROR";
                    token.value = "Invalid input";
                    tokens.push_back(token);


                }

                // Reset the lexeme and state for the next token
                lexeme = "";
                state = 0;
                continue; // Skip processing the rest of the line
            }

            // State = 0 - 7
            state = dfsm_table[state][input_index];

            // Create instance
            lexeme += c;
        }
    }

    // Check if the last lexeme is an accepted token
    if (find(accepting_states.begin(), accepting_states.end(), state) != accepting_states.end()) {
        // Create a token for the last lexeme
        token = checkStates(state, token, lexeme);

        // If the token is an identifier, insert it into the symbol table
        if (token.type == "IDENTIFIER") {
           // symTable.insert(lexeme, token.value);
        }

        // Add the token to the list of tokens
        tokens.push_back(token);
    }
    else {
        // Check if the last lexeme is to be ignored if not ERROR
        if (lexeme[0] == ' ' || lexeme[0] == '\t' || lexeme[0] == '\n' || lexeme[0] == '\0') {

        }
        else
        {
            // Invalid input, return an error token
            token.type = "ERROR";
            token.value = "Invalid input";
            tokens.push_back(token);
        }

    }

    return tokens;
}