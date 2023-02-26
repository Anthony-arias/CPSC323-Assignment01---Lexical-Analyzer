#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

// Tim's new changes

// Don't need to change this table 
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

vector<string> ops = { "==", "!=", ">", "<", "<=", "=>", "+", "-", "*", "/", "="};

vector<char> seps = { '#', '(', ')',',','{','}',';' };

struct Token {
    string type;
    string value;
};

// Reads the contents of a file into a string and returns it
string readFile(const string& fileName) {

    // Create a file variable to store input_file
    ifstream inputFile(fileName);

    // Test to see if it can open file
    if (!inputFile.is_open()) {
        cerr << "Error opening input file " << fileName << endl;
        exit(1);
    }

    // Reads the entire input file into a string buffer called input
    string input((istreambuf_iterator<char>(inputFile)), (istreambuf_iterator<char>()));

    // Closes file so no memory leaks
    inputFile.close();

    // Returns the entire input file as a string called input
    return input;
}

// Writes the contents of the token vector to an output file
void writeTokensToFile(const vector<Token>& tokens, const string& fileName) {
    // Create output file variable
    ofstream outputFile(fileName);

    // Test to see if it can open the file
    if (!outputFile.is_open()) {
        cerr << "Error opening output file " << fileName << endl;
        exit(1);
    }

    // Write entire vector to output file in a nice formatted way
    outputFile << setw(20) << left << "TOKEN" << setw(20) << left << "LEXEME" << endl << endl;
    for (const auto& token : tokens) {
        outputFile << setw(20) << left << token.type << setw(20) << left << token.value << endl;
    }
    
    // Close file so no memory leaks
    outputFile.close();
}

// Checks if char is a separtor
bool checkIfSep(char c)
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
// TODO: MAY NEED WORK ( fahr -32 ) doesnt work because of the no space on '-'
string checkIfOp(string input, int& i, bool& isOp) {
    string op = "";
    char c = input[i];

    // Check if the current character is part of a two-character operator
    if (i < input.length() - 1) {
        char next = input[i + 1];
        op = string(1, c) + next;
        if (find(ops.begin(), ops.end(), op) != ops.end()) {
            isOp = true;
            i += 2;  // Skip past the second character
            return op;
        }
    }

    // Check if the current character is a one-character operator
    op = string(1, c);
    if (find(ops.begin(), ops.end(), op) != ops.end()) {
        isOp = true;
        i++;  // Skip past the current character
        return op;
    }

    // If the current character is not an operator, return an empty string
    isOp = false;
    return "";
}

// Checks to see a block of comment so we can ignore it
int checkIfComment(char c, string input, int index)
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
bool checkIfKeyword(string lexeme)
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
Token checkStates(int state, Token token, string lexeme)
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
        }
    }
    token.value = lexeme;

    return token;
}

// Lexer function returns a vector of tokens (token.type, token.values) and goes one char
//      at a time reading the entire input_file that was given to separate each token and
//      value.
vector<Token> lexer(string input) {
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


int main(int argc, char* argv[]) {

    // Makes sure the user sends an input_file and has an output_file to write the results to
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " input_file output_file" << endl;
        exit(1);
    }

    // Read the entire input_file to the string buffer input
    string input = readFile(argv[1]);

    // Add all the tokens the lexer function returns from reading the entire input file
    vector<Token> tokens = lexer(input);

    // Write the entire vector of tokens to the specified output file. (argv[2])
    writeTokensToFile(tokens, argv[2]);

    return 0;
}
