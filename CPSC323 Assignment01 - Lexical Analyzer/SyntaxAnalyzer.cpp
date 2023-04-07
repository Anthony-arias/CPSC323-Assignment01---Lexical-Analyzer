
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


//<DeclarationList::= <Declaration>; <DeclarationList'>
bool SyntaxAnalyzer::declarationList()
{
	if (declaration())
	{
		if (syntaxTokens[current_token_index].value == ";")
		{
			current_token_index++;
			if (declarationListPrime())
				return true;
			else
				return false;
		}
	}
	else
		return false;

}

bool SyntaxAnalyzer::identifier()
{
	return false;
}


bool SyntaxAnalyzer::declarationListPrime()
{
	//if statement to check whether epsilon or functions.
	//if token is == to declaration go back in?
	//else epsilon

	if (declaration())
	{
		if (syntaxTokens[current_token_index].value == ";")
		{
			current_token_index++;
			if (declarationListPrime())
				return true;
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

bool SyntaxAnalyzer::declaration()
{
	if (qualifier())
		if (ids())
			return true;
		else return false;
	else
		return false;
}

bool SyntaxAnalyzer::ids()
{
	if (identifier())
	{
		if (idsPrime())
			return true;
		else
			return false;
	}
	else
		return false;
}
bool SyntaxAnalyzer::idsPrime()
{
	if (syntaxTokens[current_token_index].value == ",")
	{
		if (identifier())
		{
			if (idsPrime())
				return true;
			else
				return false;
		}
		else
			return false;

		return false;
	}
	else
		return false;
}


bool SyntaxAnalyzer::statementList()
{
	if (statement())
	{
		if (statementListPrime())
			return true;
		else
			return false;
	}
	else
		return false;
}

bool SyntaxAnalyzer::statementListPrime()
{
	if (statement())
	{
		if (statementListPrime())
			return true;
		else
			return false;
	}
	else
		return false;
}
//<Statement> :: = <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
bool SyntaxAnalyzer::statement()
{
	if (compound())
		return true;
	if (assign())
		return true;
	if (ifFunction())
		return true;
	if (returnFunction())
		return true;
	if (print())
		return true;
	if (scan())
		return true;
	if (whileFunction());
		return true;
}

bool SyntaxAnalyzer::compound()
{

	if (syntaxTokens[current_token_index].value == "{")
	{
		current_token_index++;
		if (statementList())
		{
			if (syntaxTokens[current_token_index].value == "}")
				return true;
			else
			{
				cout << "Error: Expected '}'."
					<< syntaxTokens[current_token_index].value << endl;
				return false;
			}
		}
		else
			return false;
	}
	else
	{
		cout << "Error: Expected '{'."
			<< syntaxTokens[current_token_index].value << endl;
		return false;
	}
}

bool SyntaxAnalyzer::assign()
{
	if (identifier())
	{
		if (syntaxTokens[current_token_index].value == "=")
		{
			current_token_index++;
			if (expression())
				return true;
			else
				return false;
		}
		else
		{
			cout << "Error: Expected '='."
				<< syntaxTokens[current_token_index].value << endl;
			return false;
		}
	}
	else
		return false;
}


//THIS NEEDS FIXING
bool SyntaxAnalyzer::ifFunction()
{
	if (syntaxTokens[current_token_index].value == "if")
	{
		current_token_index++;
		if (syntaxTokens[current_token_index].value == "(")
		{
			current_token_index++;
			if (condition())
			{
				if (syntaxTokens[current_token_index].value == ")")
				{
					current_token_index++;
					if (statement())
					{
						if (ifFunctionPrime())
							return true;
						else
							return false;
					}
					else
						return false;
				}
				else
				{
					cout << "Error: Expected ')'."
						<< syntaxTokens[current_token_index].value << endl;
					return false;
				}
			}
			else
				return false;
		}
		else
		{
			cout << "Error: Expected '('."
				<< syntaxTokens[current_token_index].value << endl;
			return false;
		}
	}
	else
	{
		cout << "Error: Expected 'if'."
			<< syntaxTokens[current_token_index].value << endl;
		return false;
	}
}
bool SyntaxAnalyzer::ifFunctionPrime()
{
	if (syntaxTokens[current_token_index].value == "fi")
	{
		current_token_index++;
		return true;
	}
	if (syntaxTokens[current_token_index].value == "else")
	{
		current_token_index++;
		if (statement())
		{
			if (syntaxTokens[current_token_index].value == "fi")
			{
				current_token_index++;
				return true;
			}
			else
				return false;
		}
	}
	else
	{
		cout << "Error: Expected 'fi'."
			<< syntaxTokens[current_token_index].value << endl;
		return false;
	}
}











//THESE NEED FIXING.
bool SyntaxAnalyzer::returnFunction()
{
	if (syntaxTokens[current_token_index].value == "return")
	{
		current_token_index++;
		return true;
	}
	else
	{
		cout << "Error: Expected return. "
			<< syntaxTokens[current_token_index].value << endl;
		return false;
	}
}
bool SyntaxAnalyzer::print()
{
	if (syntaxTokens[current_token_index].value == "print")
	{
		current_token_index++;
		return true;
	}
	else
	{
		cout << "Error: Expected print."
			<< syntaxTokens[current_token_index].value << endl;
		return false;
	}
}
bool SyntaxAnalyzer::scan()
{
	if (syntaxTokens[current_token_index].value == "scan")
	{
		current_token_index++;
		return true;
	}
	else
	{
		cout << "Error: Expected scan. "
			<< syntaxTokens[current_token_index].value << endl;
		return false;
	}
}
bool SyntaxAnalyzer::whileFunction()
{
	if (syntaxTokens[current_token_index].value == "while")
	{
		current_token_index++;
		return true;
	}
	else
	{
		cout << "Error: Expected while."
			<< syntaxTokens[current_token_index].value << endl;
		return false;
	}
}

bool SyntaxAnalyzer::expression()
{
	return true;
}