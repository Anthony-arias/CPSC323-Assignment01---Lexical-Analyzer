#include "SyntaxAnalyzer.h"

// R1. <Rat23S> ::= <Opt Function Definitions> # <Opt Declaration List> # <Statement List>
void SyntaxAnalyzer::rat23S()
{
	optFunctionDefinitions();
	if (syntaxTokens[current_token_index].value == "#")
	{
		current_token_index++;
		optDeclarationList();
		if (syntaxTokens[current_token_index].value == "#")
		{
			current_token_index++;
			statementList();
			return;
		}
		else
		{
			cout << "Error: Expected '#' after declaration list. Found: "
				<< syntaxTokens[current_token_index].value << endl;
			return;
		}
	}
	else
	{
		cout << "Error: Expected '#' after function definitions. Found: "
			<< syntaxTokens[current_token_index].value << endl;
		return;
	}

	return;
}

// R2. <Opt Function Definitions> :: = <Function Definitions> | <Empty>
void SyntaxAnalyzer::optFunctionDefinitions()
{
	functionDefinitions();

	// Otherwise, <Empty>
	return;
}

// R3. <Function Definitions> :: = <Function> < Function Definitions'>
void SyntaxAnalyzer::functionDefinitions()
{
	function();
	functionDefinitionsPrime();
}

//R4. <Function Definitions'> ::= empty | <Function> <Function Definitions'>
void SyntaxAnalyzer::functionDefinitionsPrime()
{
	function();
	functionDefinitionsPrime();

	// Otherwise Empty
	return;
}

//R5. <Function> :: = function <Identifier>(<Opt Parameter List>) < Opt Declaration List > <Body>
void SyntaxAnalyzer::function()
{
	if (syntaxTokens[current_token_index].value == "function")
	{
		current_token_index++;
		identifier();
		if (syntaxTokens[current_token_index].value == "(")
		{
			current_token_index++;
			optParameterList();
			if (syntaxTokens[current_token_index].value == ")")
			{
				current_token_index++;
				optDeclarationList();
				body();
			}
			else
			{
				cout << "Error: Expected ')' after opt parameter list. Found: "
					<< syntaxTokens[current_token_index].value << endl;
				return;
			}
		}
		else
		{
			cout << "Error: Expected '(' after identifier. Found: "
				<< syntaxTokens[current_token_index].value << endl;
			return;
		}
	}
	else
	{
		cout << "Error: Expected 'function' keyword. Found: "
			<< syntaxTokens[current_token_index].value << endl;
		return;
	}
}

//R6. <Opt Parameter List> :: = <Parameter List> | <Empty>
void SyntaxAnalyzer::optParameterList()
{
	parameterList();

	// Otherwise empty
	return;
}

//R7. <Parameter List> :: = <Parameter> < Parameter List'>
void SyntaxAnalyzer::parameterList()
{
	parameter();
	parameterListPrime();
}

//R8. <Parameter List'> ::= empty | , <Parameter> <Parameter List'>
void SyntaxAnalyzer::parameterListPrime()
{
	if (syntaxTokens[current_token_index].value == ",")
	{
		current_token_index++;
		parameter();
		parameterListPrime();
	}

	// Otherwise empty
	return;
}

//R9. <Parameter> ::= <IDs > <Qualifier>
void SyntaxAnalyzer::parameter()
{
	ids();
	qualifier();
}

// R10. <Qualifier> ::= int | bool | real
void SyntaxAnalyzer::qualifier()
{
	if (syntaxTokens[current_token_index].value == "int")
	{
		current_token_index++;
		return;
	}

	if (syntaxTokens[current_token_index].value == "bool")
	{
		current_token_index++;
		return;
	}

	if (syntaxTokens[current_token_index].value == "real")
	{
		current_token_index++;
		return;
	}

	cout << "Error: Expected qualifier to be 'int, bool, real'. Found: "
		<< syntaxTokens[current_token_index].value << endl;
	return;
}

//R11. <Body> ::= { < Statement List> }
void SyntaxAnalyzer::body()
{
	if (syntaxTokens[current_token_index].value == "{")
	{
		current_token_index++;
		statementList();
		if (syntaxTokens[current_token_index].value == "}")
		{
			return;
		}
		else
		{
			cout << "Error: Expected } after statement list. Found: "
				<< syntaxTokens[current_token_index].value << endl;
			return;
		}

	}
	else
	{
		cout << "Error: Expected { before statement list. Found: "
			<< syntaxTokens[current_token_index].value << endl;
		return;
	}
}

//R12. <Opt Declaration List> ::= <Declaration List> | <Empty>
void SyntaxAnalyzer::optDeclarationList()
{
	declarationList();

	// Otherwise empty
	return;
}


void SyntaxAnalyzer::ids()
{
	return;
}

void SyntaxAnalyzer::declarationList()
{
	return;
}

void SyntaxAnalyzer::statementList()
{
	return;
}

void SyntaxAnalyzer::identifier()
{
	return;
}
