
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
bool SyntaxAnalyzer::ifFunctionPrime()
{

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