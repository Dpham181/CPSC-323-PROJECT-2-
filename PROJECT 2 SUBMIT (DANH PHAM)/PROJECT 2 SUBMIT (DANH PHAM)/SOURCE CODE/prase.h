/*
Name:	Danh Pham
Course:	CPSC 323 SECTION 1
PRoject Name: PROJECT_2 SYNTAX ANNALYSIS
FILES CONTAIN: phrase.h, prase.h, main.cpp
PURPOSE: This program use to parse the tokens from the text file by using top-down parse desent. It gets all the tokens and push back
vector then find the gammar rule that fit to the Rat17f rule. System output to the terminal and Lexem can be find in lex.txt when u run
the execuable file(RAT17F).
*/

#ifndef PARSE_H
#define PARSE_H

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h> 
#include "phrase.h"

using namespace std;

class Alloperation
{
public:
	void start();
	void add(mylist n);
private:
	void check_length();
	void display_errors();
	void start_point();
	void Opt_Function_Definition();
	void myfunction();
	void Opt_Declaration_List();
	void Statement_List();
	void Statement_List1();
	void Opt_Parameter_List();
	void Body();
	void Write_statement();
	void Read_statement();
	void Return_statement();
	void Return_statement1();
	void Assign_statement();
	void if_statement();
	void while_statement();
	void expression_func();
	void Term();
	void IDs();
	void Factor();
	void primary1();
	void condition();
	void invalid_id();
	//all variables
	errorlist error_object;
	vector<mylist> alltokens;
	vector<errorlist>allerrors;
	int token_lenth;
	int update_index;
	int inserted_index;
	string * allcontent;
	string *alltypes;
	int * lineofcode;
	bool stop;



};
void Alloperation::add(mylist n)
{

	alltokens.push_back(n);
}
//if there is another tokens to parse	

void Alloperation::check_length()
{

	if (update_index<inserted_index - 1)
		update_index++;
	else
	{
		stop = true;

		if (allerrors.size() == 0)
		{
			cout << "...................................." << endl;
			cout << "No syntax error" << endl;

		}


	}
}
void Alloperation::invalid_id() {

	if (alltypes[update_index] == "invalid identifier") {
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		error_object.error_name = "invalid identifier, missed ( or , ";
		error_object.error_line = lineofcode[update_index];
		allerrors.push_back(error_object);
		update_index = inserted_index - 1;
		display_errors();
	}
}
void Alloperation::display_errors()
{
	stop = true;
	allcontent[update_index] = "finish";
	alltypes[update_index] = "no type";
	cout << "...................................." << endl;
	for (vector<errorlist>::iterator it = allerrors.begin(); it != allerrors.end(); ++it)
	{
		cout << "Error is:" << (*it).error_name << "\t" << "line:" << (*it).error_line << endl;

	}

}
void Alloperation::start()
{
	stop = false;

	token_lenth = alltokens.size();
	update_index = 0;
	inserted_index = 0;

	allcontent = new string[alltokens.size()];
	alltypes = new string[alltokens.size()];
	lineofcode = new int[alltokens.size()];
	for (vector<mylist>::iterator it = alltokens.begin(); it != alltokens.end(); ++it)
	{

		allcontent[inserted_index] = (*it).content;
		alltypes[inserted_index] = (*it).type;
		lineofcode[inserted_index] = (*it).line;
		inserted_index++;

	}
	start_point();
}
//it Rat17F rule
void Alloperation::start_point()
{
	Opt_Function_Definition();
	if (allcontent[update_index] == "%%")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Function Definition'> <Epsilon>"<<endl;
		cout << "\n\n";
		check_length();
		Opt_Declaration_List();
		Statement_List();

	}
	else
	{
		if (!stop)
		{

			error_object.error_name = "missed %%";
			error_object.error_line = lineofcode[update_index];
			allerrors.push_back(error_object);
			update_index = inserted_index - 1;
			display_errors();

		}
	}

}


void  Alloperation::Opt_Function_Definition()
{
first:
	if (allcontent[update_index] == "@")
	{

		myfunction();
		if (allcontent[update_index] == "@")
			goto first;
	}
	//if opt_fnction_defination -> empty that mean



}

void  Alloperation::myfunction()
{
	//we here must remove left recursive by make <Function Definitions>  ::= <Function>
	cout << "Token:" << alltypes[update_index] << "\t\t" << "Lexeme:" << allcontent[update_index] << endl;
	cout << endl;
	cout << "<RAT17F> <Opt Function Definitions>" << "%% <Opt Declaration List> <Statement List>\n";
	cout << "<Function> @ <Identifier> (<Opt Parameter List>) <Opt Declaration List> <Body>" << endl;
	check_length();
	cout << "\n\n";


	if (alltypes[update_index] == "identifier")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\n\n";

		check_length();
	afterfirstadentifier:
		if (allcontent[update_index] == "(")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;			
			cout << "\n\n";

			check_length();
		beforeparameterlist:
			Opt_Parameter_List();

			if (allcontent[update_index] == ")")
			{
				cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
				cout << "\t" << "<Parameter List'> <Epsilon>" << endl;
				cout << "\n\n";
				check_length();
			afterparameterlist:
				Opt_Declaration_List();
				Body();

			}
			else
			{

				if (!stop)
				{
					error_object.error_name = "missed ) of parameter list";
					error_object.error_line = lineofcode[update_index];
					allerrors.push_back(error_object);
					update_index = inserted_index - 1;
					display_errors();
					//  goto afterparameterlist;
				}
			}

		}
		else
		{
			if (!stop)
			{
				error_object.error_name = "missed ( of parameter list";
				error_object.error_line = lineofcode[update_index];
				allerrors.push_back(error_object);
				update_index = inserted_index - 1;
				display_errors();
				// goto beforeparameterlist;
			}
		}
	}
	else
	{
		if (!stop)
		{
			error_object.error_name = "missed identifier";
			error_object.error_line = lineofcode[update_index];
			allerrors.push_back(error_object);
			update_index = inserted_index - 1;
			display_errors();
			//goto afterfirstadentifier;
		}
	}
}
/*
in this method we do recursion desent for (opt paramater list) and  all rules we will  used
<Opt Parameter List> ::=  <Parameter List>   |  <Empty>
here it is must to remove left recursion by
<Parameter List>  ::=  <Parameter>  | <Parameter> , <Parameter List>
turned to               parameter list ::=parameter parameter list'
parameter list'::=,paramater|empty

<Parameter> ::=  <IDs > : <Qualifier>
<Qualifier> ::= integer   |  boolean  |  floating
here it is must to remove left recursion by:
<IDs> ::=  <Identifier>    | <Identifier>, <IDs>
IDs ::= identifier IDs'
IDs'::=,identifier|empty
*/
void Alloperation::Opt_Parameter_List()
{
	
first:
	
	if (alltypes[update_index] == "identifier")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Opt Parameter List> <Parameter List>" << endl;
		cout << "\t" << "<parameter list> <parameter>" << endl;
		cout << "\t" << "<Parameter> <IDs >" << endl;
		cout << "\t" << "<IDs> <identifier> <IDs'>" << endl;
		cout << "\n\n";
		check_length();
		invalid_id();
	second:
		if (allcontent[update_index] == ",")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
			cout << "\t" << "<Parameter> <IDs >" << endl;
			cout << "\t" << "<IDs> <identifier> <IDs'>" << endl;
			cout << "\t" << "<IDs'> ,<identifier> <IDs'>" << endl;
			cout << "\n\n";

			check_length();
			if (alltypes[update_index] == "identifier")
			{
				cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
				cout << "\n\n";
				check_length();
				goto second;
			}
			else
			{
				if (!stop)
				{
					error_object.error_name = "missed identifier after ,";
					error_object.error_line = lineofcode[update_index];
					allerrors.push_back(error_object);
					update_index = inserted_index - 1;
					display_errors();
					// goto second;
				}

			}

		}
		else if (allcontent[update_index] == ";")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;

			cout << "\n\n";

			check_length();
		}
		else if (allcontent[update_index] == ":")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
			cout << endl;
			cout << "\t" << "<Opt Parameter List> <Parameter List>" << endl;
			cout << "\t" << "<parameter list> <:>" << endl;
			cout << "\t" << "<Parameter> <IDs >" << endl;
			cout << "\t" << "<IDs> <identifier> <IDs'>" << endl;
			cout << "\t" << "<IDs'> < Epsilon >" << endl;
			cout << "\n\n";

			check_length();

		third:
			if (allcontent[update_index] == "integer" || allcontent[update_index] == "boolean" || allcontent[update_index] == "floating")
			{
				cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
				cout << endl;
				cout << "\t" << "<Opt Parameter List> <Parameter List>" << endl;
				cout << "\t" << "<parameter list> <qualifier>" << endl;
				cout << "\t" << "<Qualifier>" << " <" << allcontent[update_index] << ">" << endl;
				cout << "\n\n";

				check_length();
				if (allcontent[update_index] == ",")
				{

					cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
					cout << endl;
					cout << "\t" << "<Opt Parameter List> <Parameter List>" << endl;
					cout << "\t" << "<parameter list> ::=<parameter> <parameter list'>" << endl;
					cout << "\t" << "<parameter list>'::=,<paramater>" << endl;
					cout << "\n\n";

					check_length();
					if (alltypes[update_index] == "identifier")
					{
						goto first;
					}
					else
					{
						if (!stop)
						{
							error_object.error_name = "missed identifier after ,";
							error_object.error_line = lineofcode[update_index];
							allerrors.push_back(error_object);
							update_index = inserted_index - 1;
							display_errors();
							//goto second;
						}

					}
				}
			}

			else
			{

				if (!stop)
				{
					error_object.error_name = "must be one of three boolean,integer,floating after :";
					error_object.error_line = lineofcode[update_index];
					allerrors.push_back(error_object);
					update_index = inserted_index - 1;
					display_errors();
					//  goto first;
				}

			}

		}
		else
		{
			if (!stop)
			{
				error_object.error_name = "error on parameter list formating ";
				error_object.error_line = lineofcode[update_index];
				allerrors.push_back(error_object);
				update_index = inserted_index - 1;
				display_errors();
				//goto third;
			}
		}
	}
}

/*
in this method we do recursion desent for (opt Declaration list) and  all rules we will  used
<Opt Declaration List> ::= <Declaration List>   | <Empty>
<Declaration List>  := <Declaration> ;  | <Declaration> ; <Declaration List>
to remove left recursion  turn to      <Declartion list> := <Declaration>; <Declaration list'>
<Declaration list'>:=<Declaration>;|empty
<Declaration> ::=  <Qualifier > <IDs>
<Qualifier> ::= integer   |  boolean  |  floating
<IDs> ::=  <Identifier>    | <Identifier>, <IDs>
to remove left recursion  IDs ::= identifier IDs'
IDs'::=,identifier|empty

*/
void Alloperation::Opt_Declaration_List()
{

	if (allcontent[update_index] == "integer" || allcontent[update_index] == "boolean" || allcontent[update_index] == " floating")
	{

		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << endl;
		cout << "\t" << "<Opt Declaration List> <Declaration List> " << endl;
		cout << "\t" << " <Declartion list> <Declaration>; <Declaration list'>" << endl;
		cout << "\t" << " <Declaration> <Qualifier><ID>" << endl;
		cout << "\t" << "<Qualifier>" << " <" << allcontent[update_index] << ">" << endl;
		cout << "\n\n";

		check_length();
		invalid_id();

	first:
		if (alltypes[update_index] == "identifier")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
			cout << endl;
			cout << "\t" << "<Opt Declaration List> <Declaration List> " << endl;
			cout << "\t" << " <Declartion list> <Declaration>; <Declaration list'>" << endl;
			cout << "\t" << " <Declaration> <Qualifier><ID>" << endl;
			cout << "\t" << "<IDs> <identifier> <IDs'>" << endl;
			cout << "\n\n";

			check_length();
			if (allcontent[update_index] == ",")
			{
				cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
				cout << endl;
				cout << "\t" << "<Opt Declaration List> <Declaration List> " << endl;
				cout << "\t" << " <Declartion list> <Declaration>; <Declaration list'>" << endl;
				cout << "\t" << " <Declaration> <Qualifier><ID>" << endl;
				cout << "\t" << "<IDs> <identifier> <IDs'>" << endl;
				cout << "\t" << "<IDs'><,identifier>" << endl;
				cout << "\n\n";

				check_length();
				if (alltypes[update_index] == "identifier")
				{
					goto first;
				}
				else
				{
					error_object.error_name = "missed identifier";
					error_object.error_line = lineofcode[update_index];
					allerrors.push_back(error_object);
					update_index = inserted_index - 1;
					display_errors();

				}
			}
			else if (allcontent[update_index] == ";")
			{
				cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
				cout << "\t" << "<IDs'> -> Epsilon" << endl;
				cout << "\n\n";
				check_length();
				if (allcontent[update_index] == "integer" || allcontent[update_index] == "boolean" || allcontent[update_index] == " floating")
				{
					cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
					cout << "\t" << "<Opt Declaration List> <Declaration List> " << endl;
					cout << "\t" << " <Declartion list> <Declaration>; <Declaration list'>" << endl;
					cout << "\t" << " <Declaration list'> <Declaration>;" << endl;
					cout << "\t" << " <Declaration> <Qualifier><ID>" << endl;
					cout << "\t" << "<Qualifier>" << " <" << allcontent[update_index] << ">" << endl;
					cout << "\n\n";



					check_length();
					goto first;
				}




			}

			else
			{
				error_object.error_name = "must be new identifier with , or ;";
				error_object.error_line = lineofcode[update_index];
				allerrors.push_back(error_object);
				update_index = inserted_index - 1;
				display_errors();


			}



		}
		else
		{


			error_object.error_name = "must be new identifier with , or ;";
			error_object.error_line = lineofcode[update_index];
			allerrors.push_back(error_object);
			update_index = inserted_index - 1;
			display_errors();


		}

	}

}

/*
in this method we do recursion desent for (Body) and  all rules we will  used
<Body>  ::=  {  < Statement List>  }

*/
void Alloperation::Body()
{

	if (allcontent[update_index] == "{")
	{

		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Opt Declaration List><Epsilon>" << endl;
		cout << "\t" << "<Body> { <Statement List> }" << endl;
		cout << "\n\n";

		check_length();
	first:
		Statement_List();
		if (allcontent[update_index] == "}")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
			cout << "\t" << "<Statement List'>  <Epsilon>" << endl;
			cout << "\n\n";

			check_length();
		}
		else
		{

			if (!stop)
			{
				error_object.error_name = "missed } for body";
				error_object.error_line = lineofcode[update_index];
				allerrors.push_back(error_object);
				update_index = inserted_index - 1;
				display_errors();

			}



		}

	}
	else
	{
		if (!stop)
		{
			error_object.error_name = "missed { for body";
			error_object.error_line = lineofcode[update_index];
			allerrors.push_back(error_object);
			update_index = inserted_index - 1;
			display_errors();
			//  goto first;
		}


	}


}
/*
in this method we do recursion desent for (statement list s ) and  all rules we will  used
<Statement List> ::=  <Statement>   | <Statement> <Statement List>
to remove left recursion  turn to
<Statement List> ::=<Statement> <Statement List'>
<Statement List'>::<Statement>|empty

<Statement> ::=  <Compound> | <Assign> | <If> |  <Return> | <Write> | <Read> | <While>

*/
void Alloperation::Statement_List()
{

	if (allcontent[update_index] == "write")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Statement> <write>" << endl;
		cout << "\t" << "<Write> write (<Expression>);" << endl;
		cout << "\n\n";

		check_length();
		Write_statement();
		Statement_List1();

	}
	else if (allcontent[update_index] == "read")
	{

		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Statement> <read>" << endl;
		check_length();
		Read_statement();
		Statement_List1();

	}
	else if (allcontent[update_index] == "if")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Statement> <if>" << endl;
		cout << "\t" << "<If> if (<Condition>) <Statement> <If >" << endl;
		cout << "\n\n";
		check_length();
		if_statement();
		Statement_List1();

	}
	else if (allcontent[update_index] == "while")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Statement> <While>" << endl;
		cout << "\n\n";

		check_length();
		while_statement();
		Statement_List1();
	}
	else if (allcontent[update_index] == "return")
	{				
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Statement List> <Statement> <Statement List'>" << endl;
		cout << "\t" << "<Statement> <return>" << endl;		
		cout << "\t" << "<Return> return <Return'>" << endl;
		cout << "\n\n";		
		check_length();	
		Return_statement1();		
		Return_statement();
		Statement_List1();
	}
	//compound statement
	else if (allcontent[update_index] == "{")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Statement> {<compound> }" << endl;
		cout << "\t" << "<Compound> { <StatementList> }" << endl;

		cout << "\n\n";

		check_length();
		Statement_List1();
		if (allcontent[update_index] == "}")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
			cout << "\t" << "<Statement List'>  <Epsilon>" << endl;

			cout << "\n\n";

			check_length();
			Statement_List1();

		}
		else
		{

			if (!stop)
			{
				error_object.error_name = "missed } for compound";
				error_object.error_line = lineofcode[update_index];
				allerrors.push_back(error_object);
				update_index = inserted_index - 1;
				display_errors();

			}

		}

	}
	else if (alltypes[update_index] == "identifier")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Statement> <Assign>" << endl;
		cout << "\t" << "<Asign> <identifier>" << endl;
		cout << "\n\n";

		check_length();
		Assign_statement();
		Statement_List1();
	}
	else if (allcontent[update_index] == "integer" || allcontent[update_index] == "boolean" || allcontent[update_index] == "floating")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << endl;
		cout << "\t" << "<Opt Parameter List> <Parameter List>" << endl;
		cout << "\t" << "<parameter list> <qualifier>" << endl;
		cout << "\t" << "<Qualifier>" << " <" << allcontent[update_index] << ">" << endl;
		cout << "\n\n";

		check_length();
		Opt_Parameter_List();
		Statement_List();
	}
	else
	{	
		if (!stop&&allcontent[update_index] != "}")
		{
			error_object.error_name = "missed prefix of statement";
			error_object.error_line = lineofcode[update_index];
			allerrors.push_back(error_object);
			update_index = inserted_index - 1;
			display_errors();

		}

	}

}
/*
in this method we do recursion desent for (statement list s ) and  all rules we will  used
<Statement List> ::=  <Statement>   | <Statement> <Statement List>
to remove left recursion  turn to
<Statement List> ::=<Statement> <Statement List'>
<Statement List'>::<Statement>|empty

<Statement> ::=  <Compound> | <Assign> | <If> |  <Return> | <Write> | <Read> | <While>


*/
void Alloperation::Statement_List1()
{

	if (allcontent[update_index] == "write")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" <<"<Statement List> <Statement> <Statement List'>" << endl;
		cout << "\t" <<"<Statement List'> <Statement>" << endl;
		cout << "\t" <<"<Statement> <write>" << endl;
		cout << "\t" << "<Write> write(<Expression>);" << endl;
		cout << "\n\n";

		check_length();
		Write_statement();
		Statement_List();


	}
	else if (allcontent[update_index] == "read")
	{

		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Statement List> <Statement> <Statement List'>" << endl;
		cout << "\t" << "<Statement List'> <Statement>" << endl;
		cout << "\t" << "<Statement> <read>" << endl;
		cout << "\n\n";

		check_length();
		Read_statement();
		Statement_List();
	}
	else if (allcontent[update_index] == "if")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Statement List> <Statement> <Statement List'>" << endl;
		cout << "\t" << "<Statement List'> <Statement>" << endl;
		cout << "\t" << "<Statement> <if>" << endl;
		cout << "\t" << "<If> if (<Condition>) <Statement> <If'>" << endl;
		cout << "\n\n";

		check_length();
		if_statement();
		Statement_List();

	}
	else if (allcontent[update_index] == "while")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Statement List> <Statement> <Statement List'>" << endl;
		cout << "\t" << "<Statement List'> <Statement>" << endl;
		cout << "\t" << "<Statement> <while>" << endl;
		cout << "\n\n";

		check_length();
		while_statement();
		Statement_List();
	}
	else if (allcontent[update_index] == "return")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Statement List> <Statement> <Statement List'>" << endl;
		cout << "\t" << "<Statement List'> <Statement>" << endl;
		cout << "\t" << "<Statement> <return>" << endl;
		cout << "\n\n";
		check_length();
		Return_statement1();
		Return_statement();
		Statement_List();

	}
	//compound statement
	else if (allcontent[update_index] == "{")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Statement List> <Statement> <Statement List'>" << endl;
		cout << "\t" << "<Statement List'> <Statement>" << endl;
		cout << "\t" << "<Statement> {<compound> }" << endl;
		cout << "\n\n";

		check_length();
		Statement_List();
		if (allcontent[update_index] == "}")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
			cout << "\t" << "<Statement List'>  <Epsilon>" << endl;

			check_length();
			Statement_List();

		}
		else
		{
			if (!stop)
			{
				error_object.error_name = "missed } for compound";
				error_object.error_line = lineofcode[update_index];
				allerrors.push_back(error_object);
				update_index = inserted_index - 1;
				display_errors();

			}


		}

	}
	else if (alltypes[update_index] == "identifier")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Statement List> <Statement> <Statement List'>" << endl;
		cout << "\t" << "<Statement List'> <Statement>" << endl;
		cout << "\t" << "<Statement> <Assign>" << endl;
		cout << "\t" << "<Assign> <Identifier> := <Expression>; " << endl;
		cout << "\n\n";

		check_length();
		Assign_statement();
		Statement_List();
	}




}
/*in this method we do recursion desent for (Read ) and  all rules we will  used
<Read> ::=    read ( <IDs> );
<IDs> ::=  <Identifier>    | <Identifier>, <IDs>
to remove left recursion  IDs ::= identifier IDs'
IDs'::=,identifier|empty
*/
void Alloperation::Read_statement()
{
	if (allcontent[update_index] == "(")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		check_length();
		invalid_id();

	first:
		if (alltypes[update_index] == "identifier")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
			cout << "\t" << "<Statement> <read> " << endl;
			cout << "\t" << " <read> (<IDs>)" << endl;
			cout << "\t" << "<IDs> <identifier> <IDs'>" << endl;
			cout << "\n\n";

			check_length();

		second:
			if (allcontent[update_index] == ",")
			{
				cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
				cout << "\t" << " <read> (<IDs>)" << endl;
				cout << "\t" << "<IDs> <identifier> <IDs'>" << endl;
				cout << "\t" << "<IDs'> ,<identifier> " << endl;
				cout << "\n\n";

				check_length();
				if (alltypes[update_index] == "identifier")
				{
					goto first;
				}
				else
				{
					if (!stop)
					{
						error_object.error_name = "missed identifier after ,";
						error_object.error_line = lineofcode[update_index];
						allerrors.push_back(error_object);
						update_index = inserted_index - 1;
						display_errors();
						// goto second;
					}


				}
			}
			if (allcontent[update_index] == ")")
			{

				cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
				cout << "\t" << "<ID'> <Epsilon>" << endl;

				cout << "\n\n";


				check_length();
			third:
				if (allcontent[update_index] == ";")
				{
					
					cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
					cout << "\n\n";

					check_length();

				}
				else
				{
					if (!stop)
					{
						error_object.error_name = "missed ;";
						error_object.error_line = lineofcode[update_index];
						allerrors.push_back(error_object);
						update_index = inserted_index - 1;
						display_errors();

					}

				}

			}
			else
			{
				if (!stop)

				{
					error_object.error_name = "missed ) for read ";
					error_object.error_line = lineofcode[update_index];
					allerrors.push_back(error_object);
					update_index = inserted_index - 1;
					display_errors();
					//  goto third;
				}
			}
		}
		else
		{
			if (!stop)
			{
				error_object.error_name = "missed ( for read ";
				error_object.error_line = lineofcode[update_index];
				allerrors.push_back(error_object);
				update_index = inserted_index - 1;
				display_errors();
				// goto second;

			}

		}

	}
	else
	{
		if (!stop)
		{
			error_object.error_name = "missed ( or , for read ";
			error_object.error_line = lineofcode[update_index];
			allerrors.push_back(error_object);
			update_index = inserted_index - 1;
			display_errors();
			//goto first;

		}

	}

}
/*in this method we do recursion desent for (write) and  all rules we will  used
<Write> ::=   write ( <Expression>);

*/
void Alloperation::Write_statement()
{
	if (allcontent[update_index] == "(")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\n\n";
		invalid_id();

		check_length();
	first:
		expression_func();
		Statement_List1();
		if (allcontent[update_index] == ")")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
			cout << "\t" << "<Primary'> <Epsilon> " << endl;
			cout << "\t" << "<Term'> <Epsilon> " << endl;
			cout << "\t" << "<Expression'> <Epsilon> " << endl;
			cout << "\n\n";

			check_length();
		second:
			if (allcontent[update_index] == ";")
			{
				cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;

				cout << "\n\n";

				check_length();
			}
			else
			{
				if (!stop)
				{
					error_object.error_name = "missed ; ";
					error_object.error_line = lineofcode[update_index];
					allerrors.push_back(error_object);
					update_index = inserted_index - 1;
					display_errors();

				}
			}

		}
		else
		{
			if (!stop)
			{
				error_object.error_name = "missed ) for write ";
				error_object.error_line = lineofcode[update_index];
				allerrors.push_back(error_object);
				update_index = inserted_index - 1;
				display_errors();
				//goto second;

			}
		}

	}
	else
	{
		if (!stop)
		{
			error_object.error_name = "missed ( for write ";
			error_object.error_line = lineofcode[update_index];
			allerrors.push_back(error_object);
			update_index = inserted_index - 1;
			display_errors();
			//goto first;

		}
	}


}
void  Alloperation::Return_statement1() {

	if (alltypes[update_index] == "integer")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Return'> <Expression>;" << endl;
		cout << "\t" << "<Expression> <Term> <Expression'>" << endl;
		cout << "\t" << "<Term> <Factor> <Term'>" << endl;
		cout << "\t" << "<Factor> <Primary>" << endl;
		cout << "\t" << "<Primary> <integer>" << endl;
		cout << "\n\n";
		check_length();
	}
	else if (alltypes[update_index] == "floating")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Return'> <Expression>;" << endl;
		cout << "\t" << "<Expression> <Term> <Expression'>" << endl;
		cout << "\t" << "<Term> <Factor> <Term'>" << endl;
		cout << "\t" << "<Factor> <Primary>" << endl;
		cout << "\t" << "<Primary> <floating>" << endl;
		cout << "\n\n";
		check_length();
	}
}
void Alloperation::Return_statement()
{
	if (allcontent[update_index] == ";")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Statement> <Return>" << endl;
		cout << "\t" << "<Return> <return> <;>" << endl;
		cout << "\n\n";
		check_length();

	}
	else
	{
		expression_func();

		if (allcontent[update_index] == ";")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
			cout << "\t" << "<Primary'> <Epsilon>" << endl;
			cout << "\t" << "<Term'> <Epsilon>" << endl;
			cout << "\t" << "<Expression'> <Epsilon>" << endl;
			cout << "\n\n";
			check_length();
		}
		
		else
		{
			if (!stop)
			{
				error_object.error_name = "missed ; ";
				error_object.error_line = lineofcode[update_index];
				allerrors.push_back(error_object);


			}
		}


	}
}
void Alloperation::while_statement()
{
	if (allcontent[update_index] == "(")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\n\n";
		invalid_id();

		check_length();
	first:
		condition();

		if (allcontent[update_index] == ")")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
			cout << "\t" << "<Primary'> <Epsilon>" << endl;
			cout << "\t" << "<Term'> <Epsilon>" << endl;
			cout << "\t" << "<Expression'> <Epsilon>" << endl;
			cout << "\n\n";

			check_length();
		second:
			Statement_List();

		}
		else
		{
			if (!stop)
			{
				error_object.error_name = "missed ( for write ";
				error_object.error_line = lineofcode[update_index];
				allerrors.push_back(error_object);
				update_index = inserted_index - 1;
				display_errors();
				//goto second;

			}
		}



	}
	else
	{
		if (!stop)
		{
			error_object.error_name = "missed ( for while ";
			error_object.error_line = lineofcode[update_index];
			allerrors.push_back(error_object);
			update_index = inserted_index - 1;
			display_errors();
			//  goto first;

		}
	}

}

void Alloperation::if_statement()
{
	if (allcontent[update_index] == "(")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\n\n";
		invalid_id();

		check_length();
	first:
		condition();		

		if (allcontent[update_index] == ")")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
			cout << "\t" << "<Term'> <Epsilon>" << endl;
			cout << "\t" << "<Expression'> <Epsilon>" << endl;
			cout << "\n\n";
			check_length();
		second:
			Statement_List();
			if (allcontent[update_index] == "fi")
			{
				cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
				cout << "\t" << "<If'> < fi > " << endl;
				cout << "\n\n";

				check_length();
			}
			else if (allcontent[update_index] == "else")
			{
				cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
				cout << "\t" << "<Statement> <if>" << endl;
				cout << "\t" << "<If'> -> else <Statement> fi" << endl;
				cout << "\n\n";
				check_length();
				
			third:
				Statement_List();
				 if (allcontent[update_index] == "fi")
				{
					cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
					cout << "\n\n";

					check_length();
				}
				else
				{
					if (!stop)
					{
						error_object.error_name = "missed fi for else statement ";
						error_object.error_line = lineofcode[update_index];
						allerrors.push_back(error_object);
						update_index = inserted_index - 1;
						display_errors();

					}
				}

			}
			else
			{
				if (!stop)
				{
					error_object.error_name = "missed second part of if_statement ";
					error_object.error_line = lineofcode[update_index];
					allerrors.push_back(error_object);
					update_index = inserted_index - 1;
					display_errors();
					// goto first;

				}
			}

		}
		else
		{
			if (!stop)
			{
				error_object.error_name = "missed ) for condition ";
				error_object.error_line = lineofcode[update_index];
				allerrors.push_back(error_object);
				update_index = inserted_index - 1;
				display_errors();
				// goto second;

			}
		}


	}
	else
	{

		if (!stop)
		{
			error_object.error_name = "missed ( for condition ";
			error_object.error_line = lineofcode[update_index];
			allerrors.push_back(error_object);
			update_index = inserted_index - 1;
			display_errors();
			//  goto first;

		}
	}


}
void Alloperation::Assign_statement()
{
	if (allcontent[update_index] == ":=")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\n\n";
		invalid_id();

		check_length();
	first:
		expression_func();
		if (allcontent[update_index] == ";")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
			cout << "\t" << "<Primary'> <Epsilon>" << endl;
			cout << "\t" << "<Term'> <Epsilon>" << endl;
			cout << "\t" << "<Expression'> <Epsilon>" << endl;

			cout << "\n\n";

			check_length();

		}
		else
		{
			if (!stop)
			{
				error_object.error_name = "missed ; ";
				error_object.error_line = lineofcode[update_index];
				allerrors.push_back(error_object);
				update_index = inserted_index - 1;
				display_errors();

			}
		}

	}
	else
	{
		if (!stop)
		{
			error_object.error_name = "mised equal operator for assign";
			error_object.error_line = lineofcode[update_index];
			allerrors.push_back(error_object);
			update_index = inserted_index - 1;
			display_errors();
			// goto first;

		}
	}

}
/*
this method for expression
<Expression>  ::= <Expression> + <Term>  | <Expression>  - <Term>  | <Term>
<Expression> ::=<Term><Expression'>
<Expression'>::=+<Term><Expression'>|-<Term><Expression'>|empty


*/
void Alloperation::expression_func()
{
	Term();
first:
	if (allcontent[update_index] == "+")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout <<"\t"<< "<Expression> <Term><Expression'>" << endl;
		cout <<"\t"<< "<Expression'> +<Term><Expression'>" << endl;
		cout << "\n\n";

		check_length();
		Term();
		goto first;
	}
	else if (allcontent[update_index] == "-")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout <<"\t"<< "<Expression> <Term><Expression'>" << endl;
		cout <<"\t"<< "<Expression'> -<Term><Expression'>" << endl;
		cout << "\n\n";

		check_length();


		Term();
		goto first;
	}


}
/*
this method for Term
<Term>    ::=  <Term> * <Factor>  | <Term> / <Factor> |  <Factor>
<Term> ::=<Factor><Term'>
<Term'>::=/<Factor><Term'>|*<Factor><Term'>|empty


*/
void Alloperation::Term()
{
	Factor();
first:
	if (allcontent[update_index] == "*")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Term> <Factor> <Term'>" << endl;
		cout << "\t" << "<Term'> *<Factor> <Term'>" << endl;
		cout << "\n\n";

		check_length();
		Factor();
		goto first;
	}
	else if (allcontent[update_index] == "/")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Term> <Factor> <Term'>" << endl;
		cout << "\t" << "<Term'> /<Factor> <Term'>" << endl;
		cout << "\n\n";

		check_length();


		Factor();
		goto first;
	}

}
/*
this method  for factor
<Factor> ::= - <Primary>   | <Primary>
*/
void Alloperation::Factor()
{
	if (allcontent[update_index] == "-")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Factor> -<Primary>" << endl;
		check_length();
		primary1();


	}
	else
	{

		primary1();
	}


}
/*
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> ) |  <Floating>  | true | false

*/

void Alloperation::primary1()
{
	invalid_id();

	if (alltypes[update_index] == "identifier")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Expression> <Term> <Expression'>" << endl;
		cout << "\t" << "<Term> <Factor> <Term'>" << endl;
		cout << "\t" << "<Factor> <Primary>" << endl;
		cout << "\t"<<  "<Primary> <identifier><Primary'>" << endl;
		cout << "\n\n";

		check_length();
		if (allcontent[update_index] == "[")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
			cout << "\t" << "<Primary'> [<IDs>]" << endl;
			cout << "\n\n";

			check_length();
			IDs();



			if (allcontent[update_index] == "]")
			{
				cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
				cout << "\t" << "<IDs'> <Epsilon>" << endl;

				cout << "\n\n";

				check_length();
			}
			else
			{


				if (!stop)
				{
					error_object.error_name = "missed ]";
					error_object.error_line = lineofcode[update_index];
					allerrors.push_back(error_object);
					update_index = inserted_index - 1;
					display_errors();
				}


			}

		}


	}
	else if (alltypes[update_index] == "integer")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Factor> <Primary>" << endl;
		cout << "\t"<<"<Primary> <integer>" << endl;
		cout << "\n\n";
		check_length();


	}
	else if (allcontent[update_index] == "(")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Factor> <Primary>" << endl;
		cout << "\t" << "<Primary> (<Expression>)" << endl;
		cout << "\n\n";

		update_index++;
		expression_func();
		if (allcontent[update_index] == ")")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
			cout << "\t" << "<Term'> <Epsilon>" << endl;
			cout << "\t" << "<Expression'> <Epsilon>" << endl;
			cout << "\n\n";

			check_length();

		}
		else
		{

			if (!stop)
			{
				error_object.error_name = "missed )";
				error_object.error_line = lineofcode[update_index];
				allerrors.push_back(error_object);

			}


		}


	}
	else if (allcontent[update_index] == ";")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		


	}
	else if (allcontent[update_index] == "true")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout <<"\t" <<"<Expression> <Term> <Expression'>" << endl;
		cout <<"\t" <<"<Term> <Factor> <Term'>" << endl;
		cout <<"\t" <<"<Factor> <Primary>" << endl;
		cout <<"\t" <<"<Primary> <true>" << endl;
		cout << "\n\n";
		check_length();


	}
	else if (allcontent[update_index] == "false")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Expression> <Term> <Expression'>" << endl;
		cout << "\t" << "<Term> <Factor> <Term'>" << endl;
		cout << "\t" << "<Factor> <Primary>" << endl;
		cout << "\t" << "<Primary> <false>" << endl;
		cout << "\n\n";

		check_length();


	}
	else if (alltypes[update_index] == "floating")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Factor> <Primary>" << endl;
		cout << "\t" <<"<Primary> <floating>" << endl;
		cout << "\n\n";
		check_length();


	}
	/*else
	{

		if (!stop)
		{
			error_object.error_name = "error in primary";
			error_object.error_line = lineofcode[update_index];
			allerrors.push_back(error_object);

		}


	}
	*/
}
/*

*/
void Alloperation::IDs()
{
	invalid_id();

	if (alltypes[update_index] == "identifier")
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<IDs> <identifier> <IDs'>" << endl;
		cout << "\n\n";

		check_length();
	first:
		if (allcontent[update_index] == ",")
		{
			cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
			cout << "\t" << "<IDs> <identifier> <IDs'>" << endl;
			cout << "\t" << "<IDs'><,identifier>" << endl;
			check_length();
			if (alltypes[update_index] == "identifier")
			{
				goto first;
			}
			else
			{

				if (!stop)
				{
					error_object.error_name = "missed identifier after ,";
					error_object.error_line = lineofcode[update_index];
					allerrors.push_back(error_object);
					update_index = inserted_index - 1;
					display_errors();

				}



			}
		}

	}
	else
	{

		if (!stop)
		{
			error_object.error_name = "missed identifier ";
			error_object.error_line = lineofcode[update_index];
			allerrors.push_back(error_object);
			update_index = inserted_index - 1;
			display_errors();
			//  goto first;
		}



	}



}
/*
*/
void Alloperation::condition()
{
	expression_func();
	if (allcontent[update_index] == ":=" )
	{
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		check_length();
	}
	else if (allcontent[update_index] == "="||allcontent[update_index] == ">" || allcontent[update_index] == "<"|| allcontent[update_index] == "/=" || allcontent[update_index] == "=>" || allcontent[update_index] == "<=") {
		cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
		cout << "\t" << "<Primary '> <Epsilon>" << endl;
		cout << "\t" << "<Term '> <Epsilon>" << endl;
		cout << "\t" << "<Expression'> <Epsilon>" << endl;
		cout << "\t" << "<Relop> < " << allcontent[update_index] << " >" << endl;
		cout << "\n\n";

		check_length();
	}
	else
	{
		if (!stop)
		{
			error_object.error_name = "missed Relop  ";
			error_object.error_line = lineofcode[update_index];
			allerrors.push_back(error_object);
			update_index = inserted_index - 1;
			display_errors();

		}
	}
	expression_func();
}


#endif