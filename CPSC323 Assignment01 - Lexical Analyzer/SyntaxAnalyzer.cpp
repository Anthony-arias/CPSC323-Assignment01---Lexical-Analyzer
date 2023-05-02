
#include "SyntaxAnalyzer.h"


string dataType = "";
string op;
bool declarationSwitch = false;

void SyntaxAnalyzer::fileOpen(string input)
{
	file.open( input + "SAoutput.txt");
	cout << "Output written to: " << input + "SAoutput.txt" << endl;
}

void SyntaxAnalyzer::gen_instr(string op, string oprnd)
{
	if (instr_address < 1000) { // check if there is space in the array
		Instr_table[instr_address][0] = to_string(instr_address); // set the index as the label
		Instr_table[instr_address][1] = op; // set the operation
		Instr_table[instr_address][2] = oprnd; // set the operand
		instr_address++; // increment the instruction address
	}
}

int SyntaxAnalyzer::get_address(string token)
{
	try {
		// Try to parse the token as an integer
		int value = stoi(token);
		return value;
	}
	catch (invalid_argument& e) {
		// If the token is not a valid integer, look it up in the symbol table
		if (symTable.contains(token))
		{
			return symTable.getAddress(token);
		}
		else
		{
			throw runtime_error("Error: Identifier " + token + " not found in symbol table.");
		}
	}
}


int SyntaxAnalyzer::pop_jumpstack()
{	
	int addr = jump_stack.back();
	jump_stack.pop_back();
	return addr;
}

void SyntaxAnalyzer::push_jumpstack(int jump_addr)
{
	jump_stack.push_back(jump_addr);
}


void SyntaxAnalyzer::back_patch(int jump_addr)
{
	int addr = pop_jumpstack();
	Instr_table[addr][2] = to_string(jump_addr);
}

// TODO:: Write this out to an output file
void SyntaxAnalyzer::print_Instr_table(string instr_table[][3], int table_size) {
	cout << "Address\tOp\tOprnd" << endl;
	for (int i = 0; i < table_size; i++) {
		cout << instr_table[i][0] << "\t" << instr_table[i][1] << "\t" << instr_table[i][2] << endl;
	}
}



// Use when terminal value is correct and you want to output its token and value
// Also iterates the current token index
void SyntaxAnalyzer::outputTokenValueAndIterate()
{
	file <<  "Token: " << left << setw(20) << syntaxTokens[current_token_index].type  << "Lexeme: " << syntaxTokens[current_token_index].value << endl;
	file << outputString;
	outputString = "";
	current_token_index++;
}

// Throw error if token value is incorrect
void SyntaxAnalyzer::throwError(string expectedTokenType, string expectedToken) 
{
	std::string error_message = "Syntax Error: Invalid token type, Found token type '"
		+ syntaxTokens[current_token_index].type
		+ "' \nExpected token type '"
		+ expectedTokenType
		+ "' \nExpected lexeme: '"
		+ expectedToken
		+ "' \n(Found Lexeme: '"
		+ syntaxTokens[current_token_index].value
		+ "')";
	throw (error_message);
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
	try
	{
		if (printRules)
			outputString = outputString + "\t<Rat23S> ::= <Opt Function Definitions> # <Opt Declaration List> # <Statement List>\n";

		optFunctionDefinitions();
		if (syntaxTokens[current_token_index].value == "eof") return;

		if (syntaxTokens[current_token_index].value == "#")
		{
			declarationSwitch = true;
			outputTokenValueAndIterate();
			optDeclarationList();
			if (syntaxTokens[current_token_index].value == "#") outputTokenValueAndIterate();
			else throwError("SEPARATOR", "#");
		}
		if (syntaxTokens[current_token_index].value == "eof") return;
		declarationSwitch = false;
		statementList();

		if (syntaxTokens[current_token_index].value == "eof") return;
		else throwError("EOF", "$");

		file.close();
	}
	catch (string error)
	{
		cout << "Error caught: see output file" << endl;
		file << error << endl;
		file.close();
	}
}

// R2. <Opt Function Definitions> :: = <Function Definitions> | <Empty>
void SyntaxAnalyzer::optFunctionDefinitions()
{
	if (syntaxTokens[current_token_index].value == "function") {

		if (printRules)
			outputString = outputString + "\t<Opt Function Definitions> -> <Function Definitions>\n";

		functionDefinitions();
	}

	if (printRules)
		outputString = outputString + "\t<Opt Function Definitions> -> E\n";
}

// R3. <Function Definitions> :: = <Function> < Function Definitions'>
void SyntaxAnalyzer::functionDefinitions()
{
	if (printRules)
		outputString = outputString + "\t<Function Definitions> -> <Function> <Function Definitions'>\n";

	function();
	functionDefinitionsPrime();
}

// R4
void SyntaxAnalyzer::functionDefinitionsPrime()
{
	if (syntaxTokens[current_token_index].value == "function")
	{
		if (printRules)//file << "\t<Function Definitions'> -> <Function> <Function Definitions'>\n";
			outputString = outputString + "\t<Function Definitions'> -> <Function> <Function Definitions'>\n";

		function();
		functionDefinitionsPrime();
	}

	if (printRules)//file << "\t<Function Definitions'> -> E\n";
		outputString = outputString + "\t<Function Definitions'> -> E\n";
}

// R5 
void SyntaxAnalyzer::function()
{
	if (printRules)
		outputString = outputString + "\t<Function> -> function <Identifier>(<Opt Parameter List>) < Opt Declaration List > <Body>\n";

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
		if (printRules)//file << "\t<Opt Parameter List> -> <Parameter List>\n";
			outputString = outputString + "\t<Opt Parameter List> -> <Parameter List>\n";

		parameterList();
	}

	if (printRules)//file << "\t<Opt Parameter List> -> E\n";
		outputString = outputString + "\t<Opt Parameter List> -> E\n";
}

// R7
void SyntaxAnalyzer::parameterList()
{
	if (printRules)//file << "\t<Parameter List> -> <Parameter> <Parameter List'>\n";
		outputString = outputString + "\t<Parameter List> -> <Parameter> <Parameter List'>\n";

	parameter();
	parameterListPrime();
}

// R8
void SyntaxAnalyzer::parameterListPrime()
{
	if (syntaxTokens[current_token_index].value == ",")
	{
		if (printRules)//file << "\t<Parameter List'> -> , <Parameter> <Parameter List'>\n";
			outputString = outputString + "\t<Parameter List'> -> , <Parameter> <Parameter List'>\n";
		outputTokenValueAndIterate();

		parameter();
		parameterListPrime();
	}


	if (printRules)//file << "\t<Parameter List'> -> E\n";
		outputString = outputString + "\t<Parameter List'> -> E\n";
}

// R9
void SyntaxAnalyzer::parameter()
{
	if (printRules)//file << "\t<Parameter> -> <IDs> <Qualifier>\n";
		outputString = outputString + "\t<Parameter> -> <IDs> <Qualifier>\n";

	ids();
	qualifier();
}

// R10
void SyntaxAnalyzer::qualifier()
{
	if (toUpper(syntaxTokens[current_token_index].value) == "Int")
	{
		if (printRules)//file << "\t<Qualifier> -> int\n";
			outputString = outputString + "\t<Qualifier> -> int\n";

		dataType = "integer";
		outputTokenValueAndIterate();

	}
	else if (toUpper(syntaxTokens[current_token_index].value) == "Bool")
	{
		if (printRules) //file << "\t<Qualifier> -> bool\n";
			outputString = outputString + "\t<Qualifier> -> bool\n";
		dataType = "boolean";
		outputTokenValueAndIterate();
	}
	else if (toUpper(syntaxTokens[current_token_index].value) == "Real")
	{
		if (printRules) //file << "\t<Qualifier> -> real\n";
			outputString = outputString + "\t<Qualifier> -> real\n";
		dataType = "REAL";
		outputTokenValueAndIterate();
	}
	else throwError("REAL", "real");
}

// R11
void SyntaxAnalyzer::body()
{
	if (syntaxTokens[current_token_index].value == "{")
	{
		if (printRules)//file << "\t<Body> -> { < Statement List> }\n";
			outputString = outputString + "\t<Body> -> { < Statement List> }\n";
		outputTokenValueAndIterate();

		statementList();

		if (syntaxTokens[current_token_index].value == "}")
		{
			outputTokenValueAndIterate();

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
		if (printRules)//file << "\t<Opt Declaration List> -> <Declaration List>\n";
			outputString = outputString + "\t<Opt Declaration List> -> <Declaration List>\n";

		declarationList();
	}
	
	if (printRules) //file << "\t<Opt Declaration List> -> E\n";
		outputString = outputString + "\t<Opt Declaration List> -> E\n";
}

// R13
void SyntaxAnalyzer::declarationList()
{
	declaration();
	if (syntaxTokens[current_token_index].value == ";")
	{
		if (printRules)//file << "\t<Declaration List> -> <Declaration> ; <Declaration List'>\n";
			outputString = outputString + "\t<Declaration List> -> <Declaration> ; <Declaration List'>\n";
		outputTokenValueAndIterate();

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
			if (printRules)//file << "\t<Declaration List'> -> <Declaration> ; <Declaration List'>\n";
				outputString = outputString + "\t<Declaration List'> -> <Declaration> ; <Declaration List'>\n";
			outputTokenValueAndIterate();

			declarationListPrime();
		}
		else throwError("SEPARATOR", ";");
	}

	if (printRules)//file << "\t<Declaration List'> -> E\n";
		outputString = outputString + "\t<Declaration List'> -> E\n";
}

// R15
void SyntaxAnalyzer::declaration()
{
	if (printRules)//file << "\t<Declaration> -> <Qualifier> <IDs>\n";
		outputString = outputString + "\t<Declaration> -> <Qualifier> <IDs>\n";

	qualifier();
	ids();
}

// R16
void SyntaxAnalyzer::ids()
{
	if (syntaxTokens[current_token_index].type == "IDENTIFIER")
	{
		if (printRules) //file << "\t<IDs> -> <Identifier> <IDs'>\n";
			outputString = outputString + "\t<IDs> -> <Identifier> <IDs'>\n";

		if(declarationSwitch)
			symTable.insert(syntaxTokens[current_token_index].value, dataType);
		outputTokenValueAndIterate();

		idsPrime();
	}
	else throwError("IDENTIFIER", " ");
}

// R17
void SyntaxAnalyzer::idsPrime()
{
	if (syntaxTokens[current_token_index].value == ",")
	{
		if (printRules)//file << "\t<IDs'> ->  , <Identifier> <IDs'>\n";
			outputString = outputString + "\t<IDs'> ->  , <Identifier> <IDs'>\n";

		outputTokenValueAndIterate();
		if (syntaxTokens[current_token_index].type == "IDENTIFIER")
		{
			if (declarationSwitch)
				symTable.insert(syntaxTokens[current_token_index].value, dataType);
			outputTokenValueAndIterate();

			idsPrime();
		}
		else throwError("IDENTIFIER", " ");
	}
	if (printRules)//file << "\t<IDs'> ->  , <Identifier> <IDs'>\n";
		outputString = outputString + "\t<IDs'> ->  E\n";
}

// R18 
void SyntaxAnalyzer::statementList()
{
	if (printRules)//file << "\t<Statement List> -> <Statement> <Statement List'>\n";
		outputString = outputString + "\t<Statement List> -> <Statement> <Statement List'>\n";

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
		if (printRules)//file << "\t<Statement List'> -> <Statement> <Statement List'>\n";
			outputString = outputString + "\t<Statement List'> -> <Statement> <Statement List'>\n";

		statement();
		statementListPrime();
	}

	if (printRules)//file << "\t<Statement List'> -> E\n";
		outputString = outputString + "\t<Statement List'> -> E\n";
}

// R20
void SyntaxAnalyzer::statement()
{
	if (syntaxTokens[current_token_index].value == "{")
	{
		if (printRules)//file << "\t<Statement> -> <Compound>\n";
			outputString = outputString + "\t<Statement> -> <Compound>\n";

		compound();
	}
	else if (syntaxTokens[current_token_index].type == "IDENTIFIER")
	{
		if (printRules)//file << "\t<Statement> -> <Assign>\n";
			outputString = outputString +"\t<Statement> -> <Assign>\n";

		assign();
	}
	else if (syntaxTokens[current_token_index].value == "if")
	{
		if (printRules)//file << "\t<Statement> -> <If>\n";
			outputString = outputString + "\t<Statement> -> <If>\n";

		ifRule();
	}
	else if (syntaxTokens[current_token_index].value == "return")
	{
		if (printRules)//file << "\t<Statement> -> <Return>\n";
			outputString = outputString +"\t<Statement> -> <Return>\n";

		returnRule();
	}
	else if (syntaxTokens[current_token_index].value == "put")
	{
		if (printRules)//file << "\t<Statement> -> <Print>\n";
			outputString = outputString +"\t<Statement> -> <Print>\n";

		print();
	}
	else if (syntaxTokens[current_token_index].value == "get")
	{
		if (printRules)//file << "\t<Statement> -> <Scan>\n";
			outputString = outputString + "\t<Statement> -> <Scan>\n";

		scan();
	}
	else if (syntaxTokens[current_token_index].value == "while")
	{
		if (printRules)//file << "\t<Statement> -> <While>\n";
			outputString = outputString + "\t<Statement> -> <While>\n";

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
		if (printRules)//file << "\t<Compound> -> { <Statement List> }\n";
			outputString = outputString + "\t<Compound> -> { <Statement List> }\n";

		outputTokenValueAndIterate();

		statementList();

		if (syntaxTokens[current_token_index].value == "}")
		{
			outputTokenValueAndIterate();

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
		if (printRules)//file << "\t<Assign> -> <Identifier> = <Expression> ;\n";
			outputString = outputString + "\t<Assign> -> <Identifier> = <Expression> ;\n";
		save = syntaxTokens[current_token_index].value;
		outputTokenValueAndIterate();


		if (syntaxTokens[current_token_index].value == "=")
		{
			outputTokenValueAndIterate();

			expression();

			gen_instr("POPM", to_string(get_address(save)));

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
		if (printRules)//file << "\t<If> -> if ( <Condition> ) <Statement> <If'>\n";
			outputString = outputString + "\t<If> -> if ( <Condition> ) <Statement> <If'>\n";

		outputTokenValueAndIterate();

		if (syntaxTokens[current_token_index].value == "(")
		{
			outputTokenValueAndIterate();

			condition();

			if (syntaxTokens[current_token_index].value == ")")
			{
				outputTokenValueAndIterate();

				statement();

				back_patch(instr_address);

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
		if (printRules) //file << "\t<If'> -> fi\n";
			outputString = outputString + "\t<If'> -> fi\n";
		outputTokenValueAndIterate();

		// back-patch the jump instruction address
		back_patch(instr_address);
	}
	else if (syntaxTokens[current_token_index].value == "else")
	{
		if (printRules) //file << "\t<If'> -> else <Statement> fi\n";
			outputString = outputString + "\t<If'> -> else <Statement> fi\n";
		outputTokenValueAndIterate();

		// generate jump instruction
		int jump_instr_address = instr_address; // save current instruction address
		gen_instr("JMP", "nil"); // generate jump instruction with empty operand
		push_jumpstack(jump_instr_address);

		// parse statement after else keyword
		statement();

		// back-patch the jump instruction address
		back_patch(instr_address);

		// check for closing fi keyword
		if (syntaxTokens[current_token_index].value == "fi")
		{
			outputTokenValueAndIterate();
		}
		else
		{
			throwError("KEYWORD", "fi");
		}
	}
	else
	{
		throwError("KEYWORD", "else");
	}
}



// 25
void SyntaxAnalyzer::returnRule()
{
	if (syntaxTokens[current_token_index].value == "return")
	{
		if (printRules)//file << "\t<Return> -> return <Return'>\n";
			outputString = outputString + "\t<Return> -> return <Return'>\n";
		outputTokenValueAndIterate();

		returnRulePrime();
	}
	else throwError("KEYWORD", "return");

}

void SyntaxAnalyzer::returnRulePrime()
{
	if (syntaxTokens[current_token_index].value == ";")
	{
		if (printRules)//file << "\t<Return'> -> ;\n";
			outputString = outputString + "\t<Return'> -> ;\n";
		outputTokenValueAndIterate();

	}
	else
	{
		expression();
		if (syntaxTokens[current_token_index].value == ";")
		{
			if (printRules)//file << "\t<Return'> -> <Expression> ;\n";
				outputString = outputString + "\t<Return'> -> <Expression> ;\n";
			outputTokenValueAndIterate();

		}
		else throwError("SEPARATOR", ";");
	}
}

void SyntaxAnalyzer::print()
{
	if (syntaxTokens[current_token_index].value == "put")
	{
		if (printRules)//file << "\t<Print> -> put ( <Expression> );\n";
			outputString = outputString + "\t<Print> -> put ( <Expression> );\n";
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
		if (printRules)//file << "\t<Scan> -> get ( <IDs> );\n";
			outputString = outputString + "\t<Scan> -> get ( <IDs> );\n";
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
		if (printRules)//file << "\t<While> -> while ( <Condition> ) <Statement> endwhile\n";
			outputString = outputString + "\t<While> -> while ( <Condition> ) <Statement> endwhile\n";

		int addr = instr_address;
		gen_instr("LABEL", "nil");

		outputTokenValueAndIterate();

		if (syntaxTokens[current_token_index].value == "(")
		{
			outputTokenValueAndIterate();

			condition();

			if (syntaxTokens[current_token_index].value == ")")
			{
				outputTokenValueAndIterate();
				
				statement();

				gen_instr("JMP", to_string(addr));
				back_patch(instr_address);

				if (syntaxTokens[current_token_index].value == "endwhile")
				{
					outputTokenValueAndIterate();

					
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
	if (printRules)//file << "\t<Condition> -> <Expression> <Relop> <Expression>\n";
		outputString = outputString + "\t<Condition> -> <Expression> <Relop> <Expression>\n";

	expression();
	relop();
	expression();

	if (op == "<")
	{
		gen_instr("LES", "nil");
		push_jumpstack(instr_address);
		gen_instr("JMPZ", "nil");
	}
	else if (op == "==")
	{
		gen_instr("EQU", "nil");
		push_jumpstack(instr_address);
		gen_instr("JMPZ", "nil");
	}
	else if (op == "!=")
	{
		gen_instr("NEQ", "nil");
		push_jumpstack(instr_address);
		gen_instr("JMPZ", "nil");
	}
	else if (op == ">")
	{
		gen_instr("GRT", "nil");
		push_jumpstack(instr_address);
		gen_instr("JMPZ", "nil");
	}
	else if (op == "<=")
	{
		gen_instr("LEQ", "nil");
		push_jumpstack(instr_address);
		gen_instr("JMPZ", "nil");
	}
	else if (op == "=>")
	{
		gen_instr("GEQ", "nil");
		push_jumpstack(instr_address);
		gen_instr("JMPZ", "nil");
	}
	else
	{
		throwError("Expected a valid comparison operator", "(e.g. <, >, <= , => , == , != ) in <Relop>");
	}
}

void SyntaxAnalyzer::relop()
{
	if (syntaxTokens[current_token_index].value == "==")
	{
		if (printRules)//file << "\t<Relop> -> ==\n";
			outputString = outputString + "\t<Relop> -> ==\n";

		op = syntaxTokens[current_token_index].value;
		outputTokenValueAndIterate();
	}
	else if (syntaxTokens[current_token_index].value == "!=")
	{
		if (printRules)//file << "\t<Relop> -> !=\n";
			outputString = outputString + "\t<Relop> -> !=\n";

		op = syntaxTokens[current_token_index].value;
		outputTokenValueAndIterate();
		
	}
	else if (syntaxTokens[current_token_index].value == ">")
	{
		if (printRules)//file << "\t<Relop> -> >\n";
			outputString = outputString + "\t<Relop> -> >\n";

		op = syntaxTokens[current_token_index].value;
		outputTokenValueAndIterate();
	}
	else if (syntaxTokens[current_token_index].value == "<")
	{
		if (printRules)//file << "\t<Relop> -> <\n";
			outputString = outputString + "\t<Relop> -> <\n";
	
		op = syntaxTokens[current_token_index].value;
		outputTokenValueAndIterate();
	}
	else if (syntaxTokens[current_token_index].value == "<=")
	{
		if (printRules)//file << "\t<Relop> -> <=\n";
			outputString = outputString + "\t<Relop> -> <=\n";

		op = syntaxTokens[current_token_index].value;
		outputTokenValueAndIterate();
	}
	else if (syntaxTokens[current_token_index].value == "=>")
	{
		if (printRules)//file << "\t<Relop> -> =>\n";
			outputString = outputString + "\t<Relop> -> =>\n";

		op = syntaxTokens[current_token_index].value;
		outputTokenValueAndIterate();
	}
	else throwError("OPERATOR", "=>");
}

void SyntaxAnalyzer::expression()
{
	if (printRules)//file << "\t<Expression> -> <Term> <Expression'>\n";
		outputString = outputString + "\t<Expression> -> <Term> <Expression'>\n";

	term();
	expressionPrime();
}

void SyntaxAnalyzer::expressionPrime()
{
	if (syntaxTokens[current_token_index].value == "+")
	{
		if (printRules)//file << "\t<Expression'> -> + <Term> <Expression'>\n";
			outputString = outputString + "\t<Expression'> -> + <Term> <Expression'>\n";
		outputTokenValueAndIterate();


		term();
		gen_instr("ADD", "nil");
		expressionPrime();
	}
	else if (syntaxTokens[current_token_index].value == "-")
	{
		if (printRules)//file << "\t<Expression'> -> - <Term> <Expression'>\n";
			outputString = outputString + "\t<Expression'> -> - <Term> <Expression'>\n";
		outputTokenValueAndIterate();


		term();
		gen_instr("SUB", "nil");
		expressionPrime();
	}

	if (printRules) //file << "\t<Expression'> -> E\n";
		outputString = outputString + "\t<Expression'> -> E\n";
}

void SyntaxAnalyzer::term()
{

	if (printRules)//file << "\t<Term> -> <Factor> <Term'>\n";
		outputString = outputString + "\t<Term> -> <Factor> <Term'>\n";

	factor();
	termPrime();
}

void SyntaxAnalyzer::termPrime()
{
	if (syntaxTokens[current_token_index].value == "*")
	{
		if (printRules)//file << "\t<Term'> -> * <Factor> <Term'>\n";
			outputString = outputString + "\t<Term'> -> * <Factor> <Term'>\n";
		outputTokenValueAndIterate();


		factor();
		gen_instr("MUL", "nil");
		termPrime();
	}
	else if (syntaxTokens[current_token_index].value == "/")
	{
		if (printRules)//file << "\t<Term'> -> / <Factor> <Term'>\n";
			outputString = outputString + "\t<Term'> -> / <Factor> <Term'>\n";
		outputTokenValueAndIterate();

		factor();
		gen_instr("DIV", "nil");
		termPrime();
	}

	if (printRules)//file << "\t<Term'> -> E\n";
		outputString = outputString + "\t<Term'> -> E\n";
}

void SyntaxAnalyzer::factor()
{
	if (syntaxTokens[current_token_index].value == "-")
	{
		if (printRules)//file << "\t<Factor> -> - <Primary>\n";
			outputString = outputString + "\t<Factor> -> - <Primary>\n";
		outputTokenValueAndIterate();

		primary();
	}
	else
	{
		if (printRules)//file << "\t<Factor> -> <Primary>\n";
			outputString = outputString + "\t<Factor> -> <Primary>\n";

		primary();
	}
}

void SyntaxAnalyzer::primary() //++++++
{
	if (syntaxTokens[current_token_index].type == "IDENTIFIER")
	{
		if (printRules)//file << "\t<Primary> -> <Identifier>\n";
			outputString = outputString + "\t<Primary> -> <Identifier>\n";

		gen_instr("PUSHM", to_string(get_address(syntaxTokens[current_token_index].value)));
		outputTokenValueAndIterate();


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
		if (printRules)//file << "\t<Primary> -> <Integer>\n";
			outputString = outputString + "\t<Primary> -> <Integer>\n";

		gen_instr("PUSHI", to_string(get_address(syntaxTokens[current_token_index].value)));
		outputTokenValueAndIterate();
	}
	else if (syntaxTokens[current_token_index].value == "(")
	{
		if (printRules)//file << "\t<Primary> -> <Expression>\n";
			outputString = outputString + "\t<Primary> -> <Expression>\n";
		outputTokenValueAndIterate();

		expression();

		if (syntaxTokens[current_token_index].value == ")") outputTokenValueAndIterate();
		else throwError("SEPARATOR", ")");

	}
	else if (syntaxTokens[current_token_index].type == "REAL")
	{
		if (printRules)//file << "\t<Primary> -> <Real>\n";
			outputString = outputString + "\t<Primary> -> <Real>\n";
		outputTokenValueAndIterate();
	}
	else if (syntaxTokens[current_token_index].value == "true")
	{
		if (printRules)//file << "\t<Primary> -> <true>\n";
			outputString = outputString + "\t<Primary> -> <true>\n";
		outputTokenValueAndIterate();
	}
	else if (syntaxTokens[current_token_index].value == "false")
	{
		if (printRules)//file << "\t<Primary> -> <false>\n";
			outputString = outputString + "\t<Primary> -> <false>\n";
		outputTokenValueAndIterate();
	}
	else throwError("KEYWORD", "false");
}
