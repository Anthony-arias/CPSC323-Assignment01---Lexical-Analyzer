
#include "SyntaxAnalyzer.h"

// Use when terminal value is correct and you want to output its token and value
// Also iterates the current token index
void SyntaxAnalyzer::outputTokenValueAndIterate()
{
	cout << "Token: " << syntaxTokens[current_token_index].type << "Lexeme: " << syntaxTokens[current_token_index].value << endl;
	current_token_index++;
}

// Throw error if token value is incorrect
void  SyntaxAnalyzer::throwError()
{
	throw std::invalid_argument("Incorrect token value: " + syntaxTokens[current_token_index].value);
}

// R1. <Rat23S> ::= <Opt Function Definitions> # <Opt Declaration List> # <Statement List>
bool SyntaxAnalyzer::rat23S()
{
	optFunctionDefinitions();
	if (syntaxTokens[current_token_index].value == "#")
	{
		optDeclarationList();
		if (syntaxTokens[current_token_index].value == "#")
		{
			statementList();
			return true;
		}
		else 
		{
			cout << "Error: Expected '#' after declaration list. Found: "
				<< syntaxTokens[current_token_index].value << endl;
			return false;
		}
	}
	else
	{
		cout << "Error: Expected '#' after function definitions. Found: "
			<< syntaxTokens[current_token_index].value << endl;
		return false;
	}

	return true;
}

// R2. <Opt Function Definitions> :: = <Function Definitions> | <Empty>
bool SyntaxAnalyzer::optFunctionDefinitions()
{
	// Try to match <Function Definitions>
	if (functionDefinitions()) {
		return true;
	}

	// Otherwise, match <Empty>
	return true;
}

bool SyntaxAnalyzer::optDeclarationList()
{
	return false;
}

bool SyntaxAnalyzer::statementList()
{
	return false;
}

// R3. <Function Definitions> :: = <Function> < Function Definitions'>
bool SyntaxAnalyzer::functionDefinitions()
{
	if (function())
	{
		if (functionDefinitionsPrime())
		{
			return true;
		}
		else
		{
			//error
			return false;
		}
	}
	else 
	{
		//error
		return false;
	}
	
}

bool SyntaxAnalyzer::function()
{
	return false;
}

bool SyntaxAnalyzer::functionDefinitionsPrime()
{
	return false;
}

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
		if (syntaxTokens[current_token_index].value == ";")
		{
			outputTokenAndValue();
			current_token_index++;
		}
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
	primary();
}

void SyntaxAnalyzer::primary()
{
	if (syntaxTokens[current_token_index].type == "IDENTIFIER") outputTokenValueAndIterate();
	else if (syntaxTokens[current_token_index].type == "INTEGER") outputTokenValueAndIterate();
	else if (syntaxTokens[current_token_index].type == "IDENTIFIER")
	{
		outputTokenValueAndIterate();
		if (syntaxTokens[current_token_index].value == "(") outputTokenValueAndIterate();
		else throwError();

		ids();

		if (syntaxTokens[current_token_index].value == ")") outputTokenValueAndIterate();
		else throwError();
	}
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
