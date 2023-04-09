
#include "SyntaxAnalyzer.h"
#include <algorithm>

// Use when terminal value is correct and you want to output its token and value
// Also iterates the current token index
void SyntaxAnalyzer::outputTokenValueAndIterate()
{
	cout << "Token: " << syntaxTokens[current_token_index].type << "			" << "Lexeme: " << syntaxTokens[current_token_index].value << endl;
	current_token_index++;
}

// Throw error if token value is incorrect
void  SyntaxAnalyzer::throwError()
{
	throw std::invalid_argument("Incorrect token value: " + syntaxTokens[current_token_index].value);
}

// Uppercase first letter used for int, bool, real
string SyntaxAnalyzer::toUpper(std::string str) {

	transform(str.begin(), str.end(), str.begin(), ::tolower);
	str[0] = std::toupper(str[0]);

	return str;
}

// R1. <Rat23S> ::= <Opt Function Definitions> # <Opt Declaration List> # <Statement List>
void SyntaxAnalyzer::rat23S()
{
	optFunctionDefinitions();
	if (syntaxTokens[current_token_index].value == "eof") return;

	if (syntaxTokens[current_token_index].value == "#")
	{
		outputTokenValueAndIterate();
		optDeclarationList();
		if (syntaxTokens[current_token_index].value == "#") outputTokenValueAndIterate();
		else throwError();
	}
	if (syntaxTokens[current_token_index].value == "eof") return;

	statementList();

	if (syntaxTokens[current_token_index].value == "eof") return;
	else throwError();
}

// R2. <Opt Function Definitions> :: = <Function Definitions> | <Empty>
void SyntaxAnalyzer::optFunctionDefinitions()
{
	if (syntaxTokens[current_token_index].value == "function") {
		functionDefinitions();
	}
}

// R3. <Function Definitions> :: = <Function> < Function Definitions'>
void SyntaxAnalyzer::functionDefinitions()
{
	function();
	functionDefinitionsPrime();
}

// R4
void SyntaxAnalyzer::functionDefinitionsPrime()
{
	if (syntaxTokens[current_token_index].value == "function")
	{
		function();
		functionDefinitionsPrime();
	}
}

// R5 
void SyntaxAnalyzer::function()
{
	if (syntaxTokens[current_token_index].value == "function") outputTokenValueAndIterate();
	else throwError();

	if (syntaxTokens[current_token_index].type == "IDENTIFIER") outputTokenValueAndIterate();

	if (syntaxTokens[current_token_index].value == "(") outputTokenValueAndIterate();
	else throwError();

	optParameterList();

	if (syntaxTokens[current_token_index].value == ")") outputTokenValueAndIterate();
	else throwError();

	optDeclarationList();
	body();
}

// R6
void SyntaxAnalyzer::optParameterList()
{
	if (syntaxTokens[current_token_index].type == "IDENTIFIER") parameterList();
}

// R7
void SyntaxAnalyzer::parameterList()
{
	parameter();
	parameterListPrime();
}

// R8
void SyntaxAnalyzer::parameterListPrime()
{
	if (syntaxTokens[current_token_index].value == ",")
	{
		outputTokenValueAndIterate();
		parameter();
		parameterListPrime();
	}
}

// R9
void SyntaxAnalyzer::parameter()
{
	ids();
	qualifier();
}

// R10
void SyntaxAnalyzer::qualifier()
{
	if (toUpper(syntaxTokens[current_token_index].value) == "Int" ||
		toUpper(syntaxTokens[current_token_index].value) == "Bool" ||
		toUpper(syntaxTokens[current_token_index].value) == "Real") 	outputTokenValueAndIterate();
	else throwError();
}

// R11
void SyntaxAnalyzer::body()
{
	if (syntaxTokens[current_token_index].value == "{") outputTokenValueAndIterate();
	else throwError();

	statementList();

	if (syntaxTokens[current_token_index].value == "}") outputTokenValueAndIterate();
	else throwError();
}

// R12 +++++++++++++++++++++++++++++++++++
void SyntaxAnalyzer::optDeclarationList()
{
	if (toUpper(syntaxTokens[current_token_index].value) == "Int" ||
		toUpper(syntaxTokens[current_token_index].value) == "Bool" ||
		toUpper(syntaxTokens[current_token_index].value) == "Real") declarationList();
}

// R13
void SyntaxAnalyzer::declarationList()
{
	declaration();
	if (syntaxTokens[current_token_index].value == ";") outputTokenValueAndIterate();
	else throwError();
	declarationListPrime();
}

// R14 ++++++++++++++++++++++++++++++++++++++++
void SyntaxAnalyzer::declarationListPrime()
{

	if (toUpper(syntaxTokens[current_token_index].value) == "Int" ||
		toUpper(syntaxTokens[current_token_index].value) == "Bool" ||
		toUpper(syntaxTokens[current_token_index].value) == "Real")
	{
		declaration();
		if (syntaxTokens[current_token_index].value == ";") outputTokenValueAndIterate();
		else throwError();
		declarationListPrime();

	}
}

// R15
void SyntaxAnalyzer::declaration()
{
	qualifier();
	ids();
}

// R16
void SyntaxAnalyzer::ids()
{
	if (syntaxTokens[current_token_index].type == "IDENTIFIER") outputTokenValueAndIterate();
	else throwError();
	idsPrime();
}

// R17
void SyntaxAnalyzer::idsPrime()
{
	if (syntaxTokens[current_token_index].value == ",")
	{
		outputTokenValueAndIterate();
		if (syntaxTokens[current_token_index].type == "IDENTIFIER") outputTokenValueAndIterate();
		else throwError();
		idsPrime();
	}
}

// R18 
void SyntaxAnalyzer::statementList()
{
	statement();
	statementListPrime();
}

// R19 +++++++++++++++++
void SyntaxAnalyzer::statementListPrime()
{
	if (syntaxTokens[current_token_index].type == "KEYWORD" ||
		syntaxTokens[current_token_index].type == "IDENTIFIER" ||
		syntaxTokens[current_token_index].value == "{")
	{
		statement();
		statementListPrime();
	}
}

// R20
void SyntaxAnalyzer::statement()
{
	if (syntaxTokens[current_token_index].value == "{")
		compound();
	else if (syntaxTokens[current_token_index].type == "IDENTIFIER")
		assign();
	else if (syntaxTokens[current_token_index].value == "if")
		ifRule();
	else if (syntaxTokens[current_token_index].value == "return")
		returnRule();
	else if (syntaxTokens[current_token_index].value == "put")
		print();
	else if (syntaxTokens[current_token_index].value == "get")
		scan();
	else if (syntaxTokens[current_token_index].value == "while")
		whileRule();
	else
		throwError();
}

// R21
void SyntaxAnalyzer::compound()
{
	if (syntaxTokens[current_token_index].value == "{") outputTokenValueAndIterate();
	else throwError();

	statementList();

	if (syntaxTokens[current_token_index].value == "}") outputTokenValueAndIterate();
	else throwError();
}

// R22
void SyntaxAnalyzer::assign()
{
	if (syntaxTokens[current_token_index].type == "IDENTIFIER") outputTokenValueAndIterate();
	else throwError();

	if (syntaxTokens[current_token_index].value == "=") outputTokenValueAndIterate();
	else throwError();

	expression();

	if (syntaxTokens[current_token_index].value == ";") outputTokenValueAndIterate();
	else throwError();
}

// R23
void SyntaxAnalyzer::ifRule()
{
	if (syntaxTokens[current_token_index].value == "if") outputTokenValueAndIterate();
	else throwError();

	if (syntaxTokens[current_token_index].value == "(") outputTokenValueAndIterate();
	else throwError();

	condition();

	if (syntaxTokens[current_token_index].value == ")") outputTokenValueAndIterate();
	else throwError();

	statement();

	ifRulePrime();
}

// R24 ++++++++++++++++++++++++++++++
void SyntaxAnalyzer::ifRulePrime()
{
	if (syntaxTokens[current_token_index].value == "fi") outputTokenValueAndIterate();
	else if (syntaxTokens[current_token_index].value == "else")
	{
		outputTokenValueAndIterate();
		statement();
		if (syntaxTokens[current_token_index].value == "fi") outputTokenValueAndIterate();
		else throwError();
	}
	else throwError();

}

// 25
void SyntaxAnalyzer::returnRule()
{
	if (syntaxTokens[current_token_index].value == "return") outputTokenValueAndIterate();
	else throwError();
	returnRulePrime();
}

void SyntaxAnalyzer::returnRulePrime()
{
	if (syntaxTokens[current_token_index].value == ";") outputTokenValueAndIterate();
	else
	{
		expression();
		if (syntaxTokens[current_token_index].value == ";") outputTokenValueAndIterate();
		else throwError();
	}
}

void SyntaxAnalyzer::print()
{
	if (syntaxTokens[current_token_index].value == "put") outputTokenValueAndIterate();
	else throwError();

	if (syntaxTokens[current_token_index].value == "(") outputTokenValueAndIterate();
	else throwError();

	expression();

	if (syntaxTokens[current_token_index].value == ")") outputTokenValueAndIterate();
	else throwError();

	if (syntaxTokens[current_token_index].value == ";") outputTokenValueAndIterate();
	else throwError();
}

void SyntaxAnalyzer::scan()
{
	if (syntaxTokens[current_token_index].value == "get") outputTokenValueAndIterate();
	else throwError();

	if (syntaxTokens[current_token_index].value == "(") outputTokenValueAndIterate();
	else throwError();

	ids();

	if (syntaxTokens[current_token_index].value == ")") outputTokenValueAndIterate();
	else throwError();

	if (syntaxTokens[current_token_index].value == ";") outputTokenValueAndIterate();
	else throwError();
}

void SyntaxAnalyzer::whileRule()
{
	if (syntaxTokens[current_token_index].value == "while") outputTokenValueAndIterate();
	else throwError();

	if (syntaxTokens[current_token_index].value == "(") outputTokenValueAndIterate();
	else throwError();

	condition();

	if (syntaxTokens[current_token_index].value == ")") outputTokenValueAndIterate();
	else throwError();

	statement();

	if (syntaxTokens[current_token_index].value == "endwhile") outputTokenValueAndIterate();
	else throwError();
}

void SyntaxAnalyzer::condition()
{
	expression();
	relop();
	expression();
}

void SyntaxAnalyzer::relop()
{
	if (syntaxTokens[current_token_index].value == "==") outputTokenValueAndIterate();
	else if (syntaxTokens[current_token_index].value == "!=") outputTokenValueAndIterate();
	else if (syntaxTokens[current_token_index].value == ">") outputTokenValueAndIterate();
	else if (syntaxTokens[current_token_index].value == "<") outputTokenValueAndIterate();
	else if (syntaxTokens[current_token_index].value == "<=") outputTokenValueAndIterate();
	else if (syntaxTokens[current_token_index].value == "=>") outputTokenValueAndIterate();
	else throwError();
}

void SyntaxAnalyzer::expression()
{
	term();
	expressionPrime();
}

void SyntaxAnalyzer::expressionPrime()
{
	if (syntaxTokens[current_token_index].value == "+" || syntaxTokens[current_token_index].value == "-")
	{
		outputTokenValueAndIterate();
		term();
		expressionPrime();
	}
}

void SyntaxAnalyzer::term()
{
	factor();
	termPrime();
}

void SyntaxAnalyzer::termPrime()
{
	if (syntaxTokens[current_token_index].value == "*" || syntaxTokens[current_token_index].value == "/")
	{
		outputTokenValueAndIterate();
		factor();
		termPrime();
	}
}

void SyntaxAnalyzer::factor()
{
	if (syntaxTokens[current_token_index].value == "-")
	{
		outputTokenValueAndIterate();
	}
	else primary();
}

void SyntaxAnalyzer::primary() //++++++
{
	if (syntaxTokens[current_token_index].type == "IDENTIFIER")
	{
		outputTokenValueAndIterate();
		if (syntaxTokens[current_token_index].value == "(")
		{
			outputTokenValueAndIterate();
			ids();

			if (syntaxTokens[current_token_index].value == ")") outputTokenValueAndIterate();
			else throwError();
		}
	}
	else if (syntaxTokens[current_token_index].type == "INTEGER") outputTokenValueAndIterate();
	/*else if (syntaxTokens[current_token_index].type == "IDENTIFIER")
	{
		outputTokenValueAndIterate();
		if (syntaxTokens[current_token_index].value == "(") outputTokenValueAndIterate();
		else throwError();

		ids();

		if (syntaxTokens[current_token_index].value == ")") outputTokenValueAndIterate();
		else throwError();
	}*/
	else if (syntaxTokens[current_token_index].value == "(")
	{
		outputTokenValueAndIterate();

		expression();

		if (syntaxTokens[current_token_index].value == ")") outputTokenValueAndIterate();
		else throwError();
	}
	else if (syntaxTokens[current_token_index].type == "REAL") outputTokenValueAndIterate();
	else if (syntaxTokens[current_token_index].value == "true") outputTokenValueAndIterate();
	else if (syntaxTokens[current_token_index].value == "false") outputTokenValueAndIterate();
	else throwError();
}
