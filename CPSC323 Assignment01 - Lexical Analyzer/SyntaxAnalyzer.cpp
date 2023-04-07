
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
