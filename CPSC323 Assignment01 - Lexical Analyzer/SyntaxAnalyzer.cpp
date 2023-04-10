
#include "SyntaxAnalyzer.h"
#include <algorithm>


void SyntaxAnalyzer::fileOpen(string input)
{
	file.open( input + "SAoutput.txt");
}

// Use when terminal value is correct and you want to output its token and value
// Also iterates the current token index
void SyntaxAnalyzer::outputTokenValueAndIterate()
{
	file <<  "Token: " << left << setw(20) << syntaxTokens[current_token_index].type  << "Lexeme: " << syntaxTokens[current_token_index].value << endl;
	current_token_index++;
}

// Throw error if token value is incorrect
void SyntaxAnalyzer::throwError(string expectedTokenType, string expectedToken) 
{
	outputString = "Syntax Error: Invalid token type, Found token type '"
		+ syntaxTokens[current_token_index].type
		+ "' Expected token type '"
		+ expectedTokenType
		+ "' Expected lexeme: '"
		+ expectedToken
		+ "' (Found Lexeme: '"
		+ syntaxTokens[current_token_index].value
		+ "')";
	throw std::invalid_argument(outputString);
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
	if (printRules)
		file << "\t<Rat23S> ::= <Opt Function Definitions> # <Opt Declaration List> # <Statement List>\n";

	optFunctionDefinitions();
	if (syntaxTokens[current_token_index].value == "eof") return;

	if (syntaxTokens[current_token_index].value == "#")
	{
		outputTokenValueAndIterate();
		optDeclarationList();
		if (syntaxTokens[current_token_index].value == "#") outputTokenValueAndIterate();
		else throwError("SEPARATOR", "#");
	}
	if (syntaxTokens[current_token_index].value == "eof") return;

	statementList();

	if (syntaxTokens[current_token_index].value == "eof") return;
	else throwError("EOF", "$");

	file.close();
}

// R2. <Opt Function Definitions> :: = <Function Definitions> | <Empty>
void SyntaxAnalyzer::optFunctionDefinitions()
{
	if (syntaxTokens[current_token_index].value == "function") {

		if (printRules)
			file << "\t<Opt Function Definitions> -> <Function Definitions>\n";

		functionDefinitions();
	}

	if (printRules)
		file << "\t<Opt Function Definitions> -> E\n";
}

// R3. <Function Definitions> :: = <Function> < Function Definitions'>
void SyntaxAnalyzer::functionDefinitions()
{
	if (printRules)
		file << "\t<Function Definitions> -> <Function> <Function Definitions'>\n";

	function();
	functionDefinitionsPrime();
}

// R4
void SyntaxAnalyzer::functionDefinitionsPrime()
{
	if (syntaxTokens[current_token_index].value == "function")
	{
		if (printRules)
			file << "\t<Function Definitions'> -> <Function> <Function Definitions'>\n";

		function();
		functionDefinitionsPrime();
	}

	if (printRules)
		file << "\t<Function Definitions'> -> E\n";
}

// R5 
void SyntaxAnalyzer::function()
{
	if (printRules)
		file << "\t<Function> -> function <Identifier>(<Opt Parameter List>) < Opt Declaration List > <Body>\n";

	if (syntaxTokens[current_token_index].value == "function") outputTokenValueAndIterate();
	else throwError("KEYWORD", "function");

	if (syntaxTokens[current_token_index].type == "IDENTIFIER") outputTokenValueAndIterate();

	if (syntaxTokens[current_token_index].value == "(") outputTokenValueAndIterate();
	else throwError("SEPARATOR", "(");

	optParameterList();

	if (syntaxTokens[current_token_index].value == ")") outputTokenValueAndIterate();
	else throwError("SEPARATOR", "(");

	optDeclarationList();
	body();
}

// R6
void SyntaxAnalyzer::optParameterList()
{
	if (syntaxTokens[current_token_index].type == "IDENTIFIER")
	{
		if (printRules)
			file << "\t<Opt Parameter List> -> <Parameter List>\n";

		parameterList();
	}

	if (printRules)
		file << "\t<Opt Parameter List> -> E\n";
}

// R7
void SyntaxAnalyzer::parameterList()
{
	if (printRules)
		file << "\t<Parameter List> -> <Parameter> <Parameter List'>\n";

	parameter();
	parameterListPrime();
}

// R8
void SyntaxAnalyzer::parameterListPrime()
{
	if (syntaxTokens[current_token_index].value == ",")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Parameter List'> -> , <Parameter> <Parameter List'>\n";

		parameter();
		parameterListPrime();
	}


	if (printRules)
		file << "\t<Parameter List'> -> E\n";
}

// R9
void SyntaxAnalyzer::parameter()
{
	if (printRules)
		file << "\t<Parameter> -> <IDs> <Qualifier>\n";

	ids();
	qualifier();
}

// R10
void SyntaxAnalyzer::qualifier()
{
	if (toUpper(syntaxTokens[current_token_index].value) == "Int")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Qualifier> -> int\n";
	}
	else if (toUpper(syntaxTokens[current_token_index].value) == "Bool")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Qualifier> -> bool\n";
	}
	else if (toUpper(syntaxTokens[current_token_index].value) == "Real")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Qualifier> -> real\n";
	}
	else throwError("REAL", "real");
}

// R11
void SyntaxAnalyzer::body()
{
	if (syntaxTokens[current_token_index].value == "{")
	{
		outputTokenValueAndIterate();

		statementList();

		if (syntaxTokens[current_token_index].value == "}")
		{
			outputTokenValueAndIterate();

			if (printRules)
				file << "\t<Body> -> { < Statement List> }\n";
		}
		else throwError("SEPARATOR", "}");
	}
	else throwError("SEPARATOR", "{");

}

// R12 +++++++++++++++++++++++++++++++++++
void SyntaxAnalyzer::optDeclarationList()
{
	if (toUpper(syntaxTokens[current_token_index].value) == "Int" ||
		toUpper(syntaxTokens[current_token_index].value) == "Bool" ||
		toUpper(syntaxTokens[current_token_index].value) == "Real")
	{
		if (printRules)
			file << "\t<Opt Declaration List> -> <Declaration List>\n";

		declarationList();
	}
	
	if (printRules)
		file << "\t<Opt Declaration List> -> E\n";
}

// R13
void SyntaxAnalyzer::declarationList()
{
	declaration();
	if (syntaxTokens[current_token_index].value == ";")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Declaration List> -> <Declaration> ; <Declaration List'>\n";

		declarationListPrime();
	}
	else throwError("SEPARATOR", ";");
}

// R14 ++++++++++++++++++++++++++++++++++++++++
void SyntaxAnalyzer::declarationListPrime()
{

	if (toUpper(syntaxTokens[current_token_index].value) == "Int" ||
		toUpper(syntaxTokens[current_token_index].value) == "Bool" ||
		toUpper(syntaxTokens[current_token_index].value) == "Real")
	{
		declaration();
		if (syntaxTokens[current_token_index].value == ";")
		{
			outputTokenValueAndIterate();

			if (printRules)
				file << "\t<Declaration List'> -> <Declaration> ; <Declaration List'>\n";

			declarationListPrime();
		}
		else throwError("SEPARATOR", ";");
	}

	if (printRules)
		file << "\t<Declaration List'> -> E\n";
}

// R15
void SyntaxAnalyzer::declaration()
{
	if (printRules)
		file << "\t<Declaration> -> <Qualifier> <IDs>\n";

	qualifier();
	ids();
}

// R16
void SyntaxAnalyzer::ids()
{
	if (syntaxTokens[current_token_index].type == "IDENTIFIER")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<IDs> -> <Identifier> <IDs'>\n";

		idsPrime();
	}
	else throwError("IDENTIFIER", " ");
}

// R17
void SyntaxAnalyzer::idsPrime()
{
	if (syntaxTokens[current_token_index].value == ",")
	{
		outputTokenValueAndIterate();
		if (syntaxTokens[current_token_index].type == "IDENTIFIER")
		{
			outputTokenValueAndIterate();

			if (printRules)
				file << "\t<IDs'> ->  , <Identifier> <IDs'>\n";

			idsPrime();
		}
		else throwError("IDENTIFIER", " ");
	}
}

// R18 
void SyntaxAnalyzer::statementList()
{
	if (printRules)
		file << "\t<Statement List> -> <Statement> <Statement List'>\n";

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
		if (printRules)
			file << "\t<Statement List'> -> <Statement> <Statement List'>\n";

		statement();
		statementListPrime();
	}

	if (printRules)
		file << "\t<Statement List'> -> E\n";
}

// R20
void SyntaxAnalyzer::statement()
{
	if (syntaxTokens[current_token_index].value == "{")
	{
		if (printRules)
			file << "\t<Statement> -> <Compound>\n";

		compound();
	}
	else if (syntaxTokens[current_token_index].type == "IDENTIFIER")
	{
		if (printRules)
			file << "\t<Statement> -> <Assign>\n";

		assign();
	}
	else if (syntaxTokens[current_token_index].value == "if")
	{
		if (printRules)
			file << "\t<Statement> -> <If>\n";

		ifRule();
	}
	else if (syntaxTokens[current_token_index].value == "return")
	{
		if (printRules)
			file << "\t<Statement> -> <Return>\n";

		returnRule();
	}
	else if (syntaxTokens[current_token_index].value == "put")
	{
		if (printRules)
			file << "\t<Statement> -> <Print>\n";

		print();
	}
	else if (syntaxTokens[current_token_index].value == "get")
	{
		if (printRules)
			file << "\t<Statement> -> <Scan>\n";

		scan();
	}
	else if (syntaxTokens[current_token_index].value == "while")
	{
		if (printRules)
			file << "\t<Statement> -> <While>\n";

		whileRule();
	}
	else
		throwError("KEYWORD", "while");
}

// R21
void SyntaxAnalyzer::compound()
{
	if (syntaxTokens[current_token_index].value == "{")
	{
		outputTokenValueAndIterate();

		statementList();

		if (syntaxTokens[current_token_index].value == "}")
		{
			outputTokenValueAndIterate();

			if (printRules)
				file << "\t<Compound> -> { <Statement List> }\n";
		}
		else throwError("SEPARATOR", "}");
	}
	else throwError("SEPARATOR", "{");

}

// R22
void SyntaxAnalyzer::assign()
{
	if (syntaxTokens[current_token_index].type == "IDENTIFIER")
	{
		if (printRules)
			file << "\t<Assign> -> <Identifier> = <Expression> ;\n";
		outputTokenValueAndIterate();


		if (syntaxTokens[current_token_index].value == "=")
		{
			outputTokenValueAndIterate();

			expression();

			if (syntaxTokens[current_token_index].value == ";")
			{
				outputTokenValueAndIterate();

			}
			else throwError("SEPARATOR", ";");
		}
		else throwError("OPERATOR", "=");
	}
	else throwError("IDENTIFIER", " ");

}

// R23
void SyntaxAnalyzer::ifRule()
{
	if (syntaxTokens[current_token_index].value == "if")
	{
		outputTokenValueAndIterate();

		if (syntaxTokens[current_token_index].value == "(")
		{
			outputTokenValueAndIterate();

			condition();

			if (syntaxTokens[current_token_index].value == ")")
			{
				outputTokenValueAndIterate();

				if (printRules)
					file << "\t<If> -> if ( <Condition> ) <Statement> <If'>\n";

				statement();

				ifRulePrime();
			}
			else throwError("SEPARATOR", ")");
		}
		else throwError("SEPARATOR", "(");
	}
	else throwError("KEYWORD", "if");

}

// R24 ++++++++++++++++++++++++++++++
void SyntaxAnalyzer::ifRulePrime()
{
	if (syntaxTokens[current_token_index].value == "fi")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<If'> -> fi\n";
	}
	else if (syntaxTokens[current_token_index].value == "else")
	{
		outputTokenValueAndIterate();
		statement();
		if (syntaxTokens[current_token_index].value == "fi")
		{
			outputTokenValueAndIterate();

			if (printRules)
				file << "\t<If'> -> else <Statement> fi\n";
		}
		else throwError("KEYWORD", "fi");
	}
	else throwError("KEYWORD", "else");

}

// 25
void SyntaxAnalyzer::returnRule()
{
	if (syntaxTokens[current_token_index].value == "return")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Return> -> return <Return'>\n";

		returnRulePrime();
	}
	else throwError("KEYWORD", "return");

}

void SyntaxAnalyzer::returnRulePrime()
{
	if (syntaxTokens[current_token_index].value == ";")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Return'> -> ;\n";
	}
	else
	{
		expression();
		if (syntaxTokens[current_token_index].value == ";")
		{
			outputTokenValueAndIterate();

			if (printRules)
				file << "\t<Return'> -> <Expression> ;\n";
		}
		else throwError("SEPARATOR", ";");
	}
}

void SyntaxAnalyzer::print()
{
	if (syntaxTokens[current_token_index].value == "put")
	{
		outputTokenValueAndIterate();

		if (syntaxTokens[current_token_index].value == "(")
		{
			outputTokenValueAndIterate();

			expression();

			if (syntaxTokens[current_token_index].value == ")")
			{
				outputTokenValueAndIterate();

				if (syntaxTokens[current_token_index].value == ";")
				{
					outputTokenValueAndIterate();

					if (printRules)
						file << "\t<Print> -> put ( <Expression> );\n";
				}
				else throwError("SEPARATOR", ";");
			}
			else throwError("SEPARATOR", ")");
		}
		else throwError("SEPARATOR", "(");
	}
	else throwError("KEYWORD", "put");
}

void SyntaxAnalyzer::scan()
{
	if (syntaxTokens[current_token_index].value == "get")
	{
		outputTokenValueAndIterate();

		if (syntaxTokens[current_token_index].value == "(")
		{
			outputTokenValueAndIterate();

			ids();

			if (syntaxTokens[current_token_index].value == ")")
			{
				outputTokenValueAndIterate();

				if (syntaxTokens[current_token_index].value == ";")
				{
					outputTokenValueAndIterate();

					if (printRules)
						file << "\t<Scan> -> get ( <IDs> );\n";
				}
				else throwError("SEPARATOR", ";");
			}
			else throwError("SEPARATOR", ")");
		}
		else throwError("SEPARATOR", "(");

	}
	else throwError("KEYWORD", "get");

}

void SyntaxAnalyzer::whileRule()
{
	if (syntaxTokens[current_token_index].value == "while")
	{
		outputTokenValueAndIterate();

		if (syntaxTokens[current_token_index].value == "(")
		{
			outputTokenValueAndIterate();

			condition();

			if (syntaxTokens[current_token_index].value == ")")
			{
				outputTokenValueAndIterate();

				statement();

				if (syntaxTokens[current_token_index].value == "endwhile")
				{
					outputTokenValueAndIterate();

					if (printRules)
						file << "\t<While> -> while ( <Condition> ) <Statement> endwhile\n";
				}
				else throwError("KEYWORD", "endwhile");
			}
			else throwError("SEPARATOR", ")");
		}
		else throwError("SEPARATOR", "(");
	}
	else throwError("KEYWORD", "while");

}

void SyntaxAnalyzer::condition()
{
	if (printRules)
		file << "\t<Condition> -> <Expression> <Relop> <Expression>\n";

	expression();
	relop();
	expression();
}

void SyntaxAnalyzer::relop()
{
	if (syntaxTokens[current_token_index].value == "==")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Relop> -> ==\n";
	}
	else if (syntaxTokens[current_token_index].value == "!=")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Relop> -> !=\n";
	}
	else if (syntaxTokens[current_token_index].value == ">")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Relop> -> >\n";
	}
	else if (syntaxTokens[current_token_index].value == "<")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Relop> -> <\n";
	}
	else if (syntaxTokens[current_token_index].value == "<=")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Relop> -> <=\n";
	}
	else if (syntaxTokens[current_token_index].value == "=>")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Relop> -> =>\n";
	}
	else throwError("OPERATOR", "=>");
}

void SyntaxAnalyzer::expression()
{
	if (printRules)
		file << "\t<Expression> -> <Term> <Expression'>\n";

	term();
	expressionPrime();
}

void SyntaxAnalyzer::expressionPrime()
{
	if (syntaxTokens[current_token_index].value == "+")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Expression'> -> + <Term> <Expression'>\n";

		term();
		expressionPrime();
	}
	else if (syntaxTokens[current_token_index].value == "-")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Expression'> -> - <Term> <Expression'>\n";

		term();
		expressionPrime();
	}

	if (printRules)
		file << "\t<Expression'> -> E\n";
}

void SyntaxAnalyzer::term()
{

	if (printRules)
		file << "\t<Term> -> <Factor> <Term'>\n";

	factor();
	termPrime();
}

void SyntaxAnalyzer::termPrime()
{
	if (syntaxTokens[current_token_index].value == "*")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Term'> -> * <Factor> <Term'>\n";

		factor();
		termPrime();
	}
	else if (syntaxTokens[current_token_index].value == "/")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Term'> -> / <Factor> <Term'>\n";

		factor();
		termPrime();
	}

	if (printRules)
		file << "\t<Term'> -> E\n";
}

void SyntaxAnalyzer::factor()
{
	if (syntaxTokens[current_token_index].value == "-")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Factor> -> - <Primary>\n";

		primary();
	}
	else
	{
		if (printRules)
			file << "\t<Factor> -> <Primary>\n";

		primary();
	}
}

void SyntaxAnalyzer::primary() //++++++
{
	if (syntaxTokens[current_token_index].type == "IDENTIFIER")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Primary> -> <Identifier>\n";

		if (syntaxTokens[current_token_index].value == "(")
		{
			outputTokenValueAndIterate();
			ids();

			if (syntaxTokens[current_token_index].value == ")") outputTokenValueAndIterate();
			else throwError("SEPARATOR", ")");
		}
	}
	else if (syntaxTokens[current_token_index].type == "INTEGER")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Primary> -> <Integer>\n";
	}
	else if (syntaxTokens[current_token_index].value == "(")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Primary> -> <Expression>\n";

		expression();

		if (syntaxTokens[current_token_index].value == ")") outputTokenValueAndIterate();
		else throwError("SEPARATOR", ")");

	}
	else if (syntaxTokens[current_token_index].type == "REAL")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Primary> -> <Real>\n";
	}
	else if (syntaxTokens[current_token_index].value == "true")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Primary> -> <true>\n";
	}
	else if (syntaxTokens[current_token_index].value == "false")
	{
		outputTokenValueAndIterate();

		if (printRules)
			file << "\t<Primary> -> <false>\n";
	}
	else throwError("KEYWORD", "false");
}
