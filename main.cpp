<<<<<<< Updated upstream
#include "LexerAnalyzer.h"
#include "SyntaxAnalyzer.h"

// Create an instance to access lexer function
LexerAnalyzer lexerAnalyzer;
SyntaxAnalyzer syntaxAnalyzer;

// Reads the contents of a file into a string and returns it
string readFile(const string& fileName) {

    // Create a file variable to store input_file
    ifstream inputFile(fileName);

    // Test to see if it can open file
    if (!inputFile.is_open()) {
        cerr << "Error opening input file, file does not exist: " << fileName << endl;
        cout << "Press Enter to terminate program" << endl;
        cin.ignore(999, '\n');
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


int main(int argc, char* argv[]) {
    string userInput = "";
    cout << "Enter a .txt file name without extension: " << endl;
    cin >> userInput;
    cin.ignore(999, '\n');

    // Read the entire input_file to the string buffer input
    string input = readFile(userInput + ".txt");

    // Add all the tokens the lexer function returns from reading the entire input file
    vector<Token> tokens = lexerAnalyzer.lexer(input);
    syntaxAnalyzer.syntaxTokens = tokens;

    // Write the entire vector of tokens to the specified output file. (argv[2])
    writeTokensToFile(tokens, userInput + "Output.txt");
    cout << "Output written to: " << userInput + "Output.txt" << endl;

    cout << "Press Enter to terminate program" << endl;
    cin.ignore(999, '\n');

    return 0;
=======
#include "LexerAnalyzer.h"
#include "SyntaxAnalyzer.h"

// Create an instance to access lexer function
LexerAnalyzer lexerAnalyzer;
SyntaxAnalyzer syntaxAnalyzer;

// Reads the contents of a file into a string and returns it
string readFile(const string& fileName) {

    // Create a file variable to store input_file
    ifstream inputFile(fileName);

    // Test to see if it can open file
    if (!inputFile.is_open()) {
        cerr << "Error opening input file, file does not exist: " << fileName << endl;
        cout << "Press Enter to terminate program" << endl;
        cin.ignore(999, '\n');
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


int main(int argc, char* argv[]) {
    string userInput = "";
    cout << "Enter a .txt file name without extension: " << endl;
    cin >> userInput;
    cin.ignore(999, '\n');

    // Read the entire input_file to the string buffer input
    string input = readFile(userInput + ".txt");

    // Add all the tokens the lexer function returns from reading the entire input file
    vector<Token> tokens = lexerAnalyzer.lexer(input);
    syntaxAnalyzer.syntaxTokens = tokens;

    // Write the entire vector of tokens to the specified output file. (argv[2])
    writeTokensToFile(tokens, userInput + "Output.txt");
    cout << "Output written to: " << userInput + "Output.txt" << endl;

    cout << "Press Enter to terminate program" << endl;
    cin.ignore(999, '\n');

    return 0;
>>>>>>> Stashed changes
}