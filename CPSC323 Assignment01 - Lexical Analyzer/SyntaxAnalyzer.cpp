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
			cout << "Error: Expected another function definition after the current one. Found: "
				<< syntaxTokens[current_token_index].value << endl;
			return false;
		}
	}
	else
	{
		cout << "Error: Expected a function definition. Found: "
			<< syntaxTokens[current_token_index].value << endl;
		return false;
	}

}

//R4. <Function Definitions'> ::= empty | <Function> <Function Definitions'>
bool SyntaxAnalyzer::functionDefinitionsPrime()
{
	if (function())
	{
		if (functionDefinitionsPrime())
		{
			return true;
		}
		else
		{
			cout << "Error: Expected another function definition after the current one. Found: "
				<< syntaxTokens[current_token_index].value << endl;
			return false;
		}
	}
	else
	{
		cout << "Error: Expected a function definition. Found: "
			<< syntaxTokens[current_token_index].value << endl;
		return false;
	}

	return true;
}

//R5. <Function> :: = function <Identifier>(<Opt Parameter List>) < Opt Declaration List > <Body>
bool SyntaxAnalyzer::function()
{
	if (syntaxTokens[current_token_index].value == "function")
	{
		current_token_index++;
		if (identifier())
		{
			if (syntaxTokens[current_token_index].value == "(")
			{
				current_token_index++;
				if (optParameterList())
				{
					if (syntaxTokens[current_token_index].value == ")")
					{
						current_token_index++;
						if (optDeclarationList())
						{
							if (body())
							{
								return true;
							}
							else
							{
								cout << "Error: Expected body of function. Found: "
									<< syntaxTokens[current_token_index].value << endl;
							}
						}
						else
						{
							cout << "Error: Expected optional declaration list. Found: "
								<< syntaxTokens[current_token_index].value << endl;
						}
					}
					else
					{
						cout << "Error: Expected ')' after parameter list. Found: "
							<< syntaxTokens[current_token_index].value << endl;
					}
				}
				else
				{
					cout << "Error: Expected optional parameter list. Found: "
						<< syntaxTokens[current_token_index].value << endl;
				}
			}
			else
			{
				cout << "Error: Expected '(' after function identifier. Found: "
					<< syntaxTokens[current_token_index].value << endl;
			}
		}
		else
		{
			cout << "Error: Expected function identifier. Found: "
				<< syntaxTokens[current_token_index].value << endl;
		}
	}
	else
	{
		cout << "Error: Expected 'function' keyword. Found: "
			<< syntaxTokens[current_token_index].value << endl;
	}

	return false;
}

//R6. <Opt Parameter List> :: = <Parameter List> | <Empty>
bool SyntaxAnalyzer::optParameterList()
{
	if (parameterList())
	{
		return true;
	}

	return true;
}

//R7. <Parameter List> :: = <Parameter> < Parameter List'>
bool SyntaxAnalyzer::parameterList()
{
	if (parameter())
	{
		if (parameterListPrime())
		{
			return true;
		}
		else
		{
			//error parameterListPrime expected
			return false;
		}
	}
	else
	{
		//error parameter expected
		return false;
	}
}

//R8. <Parameter List'> ::= empty | , <Parameter> <Parameter List'>
bool SyntaxAnalyzer::parameterListPrime()
{
	if (syntaxTokens[current_token_index].value == ",")
	{
		current_token_index++;
		if (parameter())
		{
			if (parameterListPrime())
			{
				return true;
			}
			else
			{
				//expected parameterlist
				return false;
			}
		}
		else
		{
			//expected paramter
			return false;
		}
	}

	return true;
}

//R9. <Parameter> ::= <IDs > <Qualifier>
bool SyntaxAnalyzer::parameter()
{
	if (ids())
	{
		if (qualifier())
		{
			return true;
		}
	}
	else
	{
		return false;
	}

	return false;

}

// R10. <Qualifier> ::= int | bool | real
bool SyntaxAnalyzer::qualifier()
{
	if (syntaxTokens[current_token_index].value == "int")
	{
		current_token_index++;
		return true;
	}

	if (syntaxTokens[current_token_index].value == "bool")
	{
		current_token_index++;
		return true;
	}

	if (syntaxTokens[current_token_index].value == "real")
	{
		current_token_index++;
		return true;
	}

	cout << "Error: Expected qualifier int, bool, real. Found: "
		<< syntaxTokens[current_token_index].value << endl;
	return false;
}

//R11. <Body> ::= { < Statement List> }
bool SyntaxAnalyzer::body()
{
	if (syntaxTokens[current_token_index].value == "{")
	{
		current_token_index++;
		if (statementList())
		{
			if (syntaxTokens[current_token_index].value == "}")
			{
				current_token_index++;
			}
			else
			{
				cout << "Error: Expected }. Found: "
					<< syntaxTokens[current_token_index].value << endl;
				return false;
			}
		}
		else
		{
			cout << "Error: Expected statement list. Found: "
				<< syntaxTokens[current_token_index].value << endl;
			return false;
		}
	}
	else
	{
		cout << "Error: Expected {. Found: "
			<< syntaxTokens[current_token_index].value << endl;
		return false;
	}
}

//R12. <Opt Declaration List> ::= <Declaration List> | <Empty>
bool SyntaxAnalyzer::optDeclarationList()
{
	if (declarationList())
	{
		return true;
	}

	return true;
}


bool SyntaxAnalyzer::ids()
{
	return false;
}

bool SyntaxAnalyzer::declarationList()
{
	return false;
}

bool SyntaxAnalyzer::statementList()
{
	return false;
}

bool SyntaxAnalyzer::identifier()
{
	return false;
}
