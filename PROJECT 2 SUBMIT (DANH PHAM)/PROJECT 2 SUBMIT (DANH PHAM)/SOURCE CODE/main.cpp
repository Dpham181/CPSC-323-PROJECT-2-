/*
Name:	Danh Pham 
Course:	CPSC 323 SECTION 1
PRoject Name: PROJECT_2 SYNTAX ANNALYSIS 
FILES CONTAIN: phrase.h, prase.h, ma
PURPOSE: This program use to parse the tokens from the text file by using top-down parse desent. It gets all the tokens and push back 
vector then find the gammar rule that fit to the Rat17f rule. System output to the terminal and Lexem can be find in lex.txt when u run
the execuable file(RAT17F).
*/

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <string>
#include<vector>
#include "prase.h"


using namespace std;

vector<mylist> alltokens;
Alloperation myoperations;
int line_number = 1;
// define row col with const int 
const int i_l = 2;
const int i_r = 3;
const int f_c = 3;
const int f_w = 4;
const int n_kw = 14;
const int n_s = 9;
const int n_o = 10;
const int i_ac = 2;
const int f_ac = 4;
const int id_r = 5;
const int id_c = 3;

// keywords  table
string keywords[n_kw] = { "integer","if","else","fi","while","read","write","return","boolean","floating","double","true","false",
"assign" };

// sperators  table
char sperators[n_s] = { '<', '>', '{', '}', '[', ']', '(', ')',',' };

string valid_sp[1] = { "%%"};
//  operators table
char operators[n_o] = { '*', '^', ':', '.','=', '-', '+', '/', '@',';' };

//integer state table
char int_table[i_l] = { '0', 'D' };
int intState[i_r][i_l] = { { 0, 2 },{ 1, 2 },{ 2, 2 } };

//real state table
char f_table[f_c] = { 'D','.', 'D' };
int f_state[f_w][f_c] = { { 1, 2,0 },{ 2,2,3 },{ 3, 4,0 },{ 4,4,0 } };

// id state table 

char id_table[id_c] = { '0','l','#' };
int id_state[id_r][id_c] = {
	{ 1, 2,0 },
	{ 2, 3,4 },
	{ 4, 5,0 },
	{ 3, 3,4 },
	{ 5,3,4 } };



// all function bool and int declearation for very cases.
void Lexer(const string& aLine, stringstream& ssToken);
char Next_character(const string& input, int charPos);
bool check_identifier(string x);
bool isKeyword(const string& input, int charPos, int& kewwordLen, string& strKeyword);
bool isIdentifier(const string& input, int charPos, int& idLength, string& strIdent);
bool isIdentifier1(const string& input, int charPos, int& idLength, string& strIdent);
bool isequal(const string& input, int charPos, string& strIdent);
bool isnumber(const string& input, int charPos, int& idLength, string& strIdent);
int getidStatTabCol(char inputChar);
bool is_sp(const string& input, int charPos, string& strsp);
bool is_op(const string& input, int charPos, string& strOtherPunc);
bool isInt(const string& input, int charPos, int& intLength, string& strInt);
int getInt_Col(char inputChar);
bool isreal(const string& input, int charPos, int& floatLength, string& strFLoat);
int getreal_Col(char inputChar);
void printFile(const string& fileName)
{
	ifstream inFile;
	stringstream ss;

	inFile.open(fileName);
	if (inFile)
	{
		ss << inFile.rdbuf();
		cout << ss.str();
	}
	inFile.close();
}
// check %% as a valid op 
bool valid_muitilchar(const string& input, int charPos, string& valid_op)
{
	string str = input.substr(charPos, 2);
	int i = 0;
	while (str != valid_sp[i] && i < 1)
		i++;
	valid_op = valid_sp[i];
	return (i < 1);
}
int main()
{


	string inputLine = "", sourceFile = "";
	stringstream ssTokens;
	ifstream inFile;
	ofstream outFile("lexcheck.txt");
	cout << "Input file name: ";
	cin >> sourceFile;
	cout << endl;
	cout << "\tSource Code:  \n";
	printFile(sourceFile);
	cout << endl;
	inFile.open(sourceFile);


	if (inFile)
	{
		cout << endl;
		outFile << "TOken \t\t"; outFile << "Lexeme \n";
		cout << endl;
		while (!inFile.eof())
		{
			getline(inFile, inputLine);
			outFile << inputLine << endl;;

			Lexer(inputLine, ssTokens);
			line_number++;

		}
		outFile << ssTokens.str();
		cout << endl;

		inFile.close();
	}
	else
		cout << "File " << sourceFile << " not found.\n";

	cout << "............................................................\n\n" << endl;
	cout << "\t\tSyntax rules\n\n " << endl;
	myoperations.start();


	system("pause");
	return 0;
}

bool check_identifier(string x)
{
	bool checked = true;
	for (int i = 0; i<x.length(); i++)
	{
		if (x[0] == '#') {
			checked = false;
			break;
		}

		if (x[i] == '#')
		{
			
			if (i<x.length() - 1)
			{
				i++;
				if (x[i] == '#')
				{
					checked = false;
					break;
				}

			}


		}

	}
	return checked;

}


/*LEXER funtions to a list of tokens stored in a stringstream ss*/
void Lexer(const string& aLine, stringstream& ssToken)
{

	string inputLine = aLine;
	string  strsp = "", strOtherPunc = "", strequal = "", strInt = "", strFloat = "", strKeyword = "", strIdent = "", valid_op = "", number = "";
	int tokenLen = 0;
	string token = "";
	int strLen = 0, intLen = 0, floatLen = 0, keywordLen = 0, identNum = 0, idLength = 0, numlenth;

	while (inputLine.length() > 0)
	{

		tokenLen = 0;
		token = "";
		if (inputLine[0] == ' ' || inputLine[0] == '\t' || inputLine[0] == '\n')
			tokenLen = 1;
		else
		{

			if (isKeyword(inputLine, 0, keywordLen, strKeyword))
			{
				token = "keywords  \t" + strKeyword;
				tokenLen = keywordLen;
				mylist m;
				m.content = strKeyword;
				m.type = "keyword";
				m.line = line_number;
				myoperations.add(m);


			}
			else if (isIdentifier1(inputLine, 0, idLength, strIdent))
			{

				if (check_identifier(strIdent))
				{
					tokenLen = idLength;
					token = "identifier  \t" + strIdent;
					mylist m;
					m.content = strIdent;
					m.type = "identifier";
					m.line = line_number;
					myoperations.add(m);
				}
				else
				{
					tokenLen = idLength;
					token = "invalid identifier  \t" + strIdent;
					mylist m;
					m.content = strIdent;
					m.type = "invalid identifier";
					m.line = line_number;
					myoperations.add(m);



				}
			}

			else if (valid_muitilchar(inputLine, 0, valid_op))
			{
				token = "Valid operator:\t" + valid_op;
				tokenLen = 2;
				mylist m;
				m.content = valid_op;
				m.type = "valid operator";
				m.line = line_number;
				myoperations.add(m);

			}

			else if (is_sp(inputLine, 0, strsp))
			{
				token = "seperator:\t" + strsp;
				tokenLen = 1;
				mylist m;
				m.content = strsp;
				m.type = "seperatorr";
				m.line = line_number;
				myoperations.add(m);
			}
			else if (isnumber(inputLine, 0, numlenth, number))
			{
				string n = ".";

				if (strstr(number.c_str(), n.c_str()))
				{

					token = "floating:\t\t" + number;
					tokenLen = numlenth;
					mylist m;
					m.content = number;
					m.type = "floating";
					m.line = line_number;
					myoperations.add(m);


				}
				else
				{
					token = "integer:\t\t" + number;
					tokenLen = numlenth;
					mylist m;
					m.content = number;
					m.type = "integer";
					m.line = line_number;
					myoperations.add(m);


				}



			}


			else if (isequal(inputLine, 0, strequal))
			{
				token = "operator\t" + strequal;

				mylist m;
				m.content = strequal;
				m.type = "operator";
				m.line = line_number;
				myoperations.add(m);
				tokenLen = 2;

			}

			else if (is_op(inputLine, 0, strOtherPunc))
			{


				token = "operator\t" + strOtherPunc;

				mylist m;
				m.content = strOtherPunc;
				m.type = "operator";
				m.line = line_number;
				myoperations.add(m);
				tokenLen = 1;
			}

			(token.length() > 0 ? (ssToken << token << "\n") : (ssToken << ""));
		}
		strLen = inputLine.length() - tokenLen;
		inputLine = inputLine.substr(tokenLen, strLen);
	}
}

// check next char
char Next_character(const string& input, int charPos)
{
	return input[charPos + 1];
}

// check if the input string from position charPos contains a token of keyword or not. If yes, function return true an put the token in the string strKeyword
bool isKeyword(const string& input, int charPos, int& keywordLen, string& strKeyword)
{
	char ch = input[charPos];
	string token = "";
	int i = 0;
	while (isalpha(ch) && i < static_cast<int>(input.length()))
	{
		token += ch;
		ch = input[++i];
	}
	i = 0;
	while (token != keywords[i] && i < n_kw)
		i++;
	if (token == keywords[i])
	{
		keywordLen = static_cast<int>(token.length());
		strKeyword = token;
		return true;
	}
	else
		return false;
}
bool isnumber(const string& input, int charPos, int& idLength, string& strIdent)
{
	char ch = input[charPos];
	string token = "";
	int i = 0;
	while (isalnum(ch) || ch == '.' && i < static_cast<int>(input.length()))
	{
		token += ch;
		ch = input[++i];
	}
	if (token.length()>0)
	{
		idLength = static_cast<int>(token.length());
		strIdent = token;
		return true;
	}
	else
		return false;





}
bool isIdentifier1(const string& input, int charPos, int& idLength, string& strIdent)
{
	char ch = input[charPos];
	string token = "";
	int i = 0;
	while (isalpha(ch) || ch == '#' && i < static_cast<int>(input.length()))
	{
		token += ch;
		ch = input[++i];
	}
	i = 0;
	//while (token != keywords[i] && i < n_kw)
	//i++;
	if (token.length()>0)
	{
		idLength = static_cast<int>(token.length());
		strIdent = token;
		return true;
	}
	else
		return false;


}

bool isIdentifier(const string& input, int charPos, int& idLength, string& strIdent)
{
	int state = 1;

	char ch = input[charPos];
	string token = "";
	int i = 0;
	if (isalpha(ch) || ch == '#')
	{
		int i = charPos;
		bool stop = false;
		while (!stop && (isalpha(ch) || ch == '#') && i < static_cast<int>(input.length()))
		{
			ch = input[i];
			int col = getidStatTabCol(ch);
			if (col != -1)
			{
				state = id_state[state][col];
				i++;
			}
			else // col = -1
			{
				stop = true;
			}
		}
		if (i > charPos && state == 2 || state == 3 || state == 4 || state == 5)
		{
			idLength = i;
			strIdent = input.substr(charPos, i);
			return true;
		}
		else
			return false;
	}
	else
		return false;

}
// find column number of the indentifer stat table by the inputChar

int getidStatTabCol(char inputChar)

{

	if (isalpha(inputChar))
		return 3;

	else if (inputChar == '#')
		return 4;

	else
		return -1;

}


// check if the input string from position charPos contains a token of sperators or not. If yes, function return true an put the token in the string strPairedDelim
bool is_sp(const string& input, int charPos, string& strsp)
{
	char ch = input[charPos];
	int i = 0;
	while (ch != sperators[i] && i < n_s)
		i++;
	strsp = +ch;
	return (i < n_s);
}
//if := or not
bool isequal(const string& input, int charPos, string& strsp)
{
	if (input.size() >= 2)
	{
		if (input[0] == ':'&&input[1] == '=')
		{

			strsp = ":=";
			return true;
		}
		else if (input[0] == '='&&input[1] == '>')
		{

			strsp = "=>";
			return true;
		}
		else if (input[0] == '<'&&input[1] == '=')
		{

			strsp = "<=";
			return true;
		}
		else if (input[0] == '/'&&input[1] == '=')
		{

			strsp = "/=";
			return true;
		}
		else 
		{

			return false;
		}



	}
	else
	{
		return false;
	}

}


// check if the input string from position charPos contains a token of operators  or not. If yes, function return true an put the token in the string strOtherPunc
bool is_op(const string& input, int charPos, string& strOtherPunc)
{
	char ch = input[charPos];
	int i = 0;
	while (ch != operators[i] && i < n_o)
	{
		i++;
	}
	strOtherPunc = +ch;
	return (i < n_o);
}

// check if the input string from position charPos contains a token of an integer or not. If yes, function return true an put the token in the string strInt
bool isInt(const string& input, int charPos, int& intLength, string& strInt)
{
	int state = 0;
	strInt = "";
	char ch = input[charPos];
	int i = charPos;
	bool stop = false;
	if (isdigit(ch) || ch == '0') {
		while (!stop && isdigit(ch) && i < static_cast<int>(input.length()))
		{
			ch = input[i];
			int col = getInt_Col(ch);
			if (col != -1)
			{
				state = intState[state][col];
				i++;
			}
			else // col = -1
			{
				stop = true;
			}
		}
		if (i > charPos && state == i_ac)
		{
			intLength = i;
			strInt = input.substr(charPos, i);
			return true;
		}
		else
			return false;

	}
	else
		return false;
}

// find column number of the Integer stat table by the inputChar
int getInt_Col(char inputChar)
{
	if (inputChar == '0')
		return 0;
	else if (isdigit(inputChar))
		return 1;
	else
		return -1;
}

// check if the input string from position charPos contains a token of a real or not. If yes, function return true an put the token in the string strFloat
bool isreal(const string& input, int charPos, int& floatLength, string& strFLoat)
{
	int state = 1;
	strFLoat = "";
	char ch = input[charPos];
	if (isdigit(ch) || ch == '.')
	{
		int i = charPos;
		bool stop = false;
		while (!stop && (ch = '0' || isdigit(ch) || ch == '.') && i < static_cast<int>(input.length()))
		{
			ch = input[i];
			int col = getreal_Col(ch);
			if (col != -1)
			{
				state = f_state[state][col];
				i++;
			}
			else // col = -1
			{
				stop = true;
			}
		}
		if (i > charPos && state == f_ac)
		{
			floatLength = i;
			strFLoat = input.substr(charPos, i);
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

// find column number of the real stat table by the inputChar
int getreal_Col(char inputChar)
{

	if (isdigit(inputChar))
		return 1;
	else if (inputChar == '.')
		return 2;
	else
		return -1;
}