
#include "SyntaxAnalyzer.h"

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


//Rule 13-24
bool SyntaxAnalyzer::declarationList()
{
	//Goes to declaration then declarationrListPrime
	declaration();
	declarationListPrime();
}


bool SyntaxAnalyzer::declarationListPrime()
{
	//if statement to check whether epsilon or functions.
	//if token is == to declaration go back in?
	//else epsilon
	if (syntaxTokens[current_token_index].value == "....")
	{
		declaration();
		declarationListPrime();
	}
	else
		return true; // epsilon
}

bool SyntaxAnalyzer::declaration()
{
	qualifier();
	ids();
}

//Calls identifier
bool SyntaxAnalyzer::ids()
{
	// call identifier/keywords from assignment 1
	idsPrime();
}
bool SyntaxAnalyzer::idsPrime()
{
	//if identifier || idsprime
	//else return something (epsilon)
}