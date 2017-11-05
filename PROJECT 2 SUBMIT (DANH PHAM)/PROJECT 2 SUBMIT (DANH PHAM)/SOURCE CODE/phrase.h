
/*
Name:	Danh Pham
Course:	CPSC 323 SECTION 1
PRoject Name: PROJECT_2 SYNTAX ANNALYSIS
FILES CONTAIN: phrase.h, prase.h, main.cpp
PURPOSE: This program use to parse the tokens from the text file by using top-down parse desent. It gets all the tokens and push back
vector then find the gammar rule that fit to the Rat17f rule. System output to the terminal and Lexem can be find in lex.txt when u run
the execuable file(RAT17F).
*/




#ifndef PHARSE_H
#define PHARSE_H
#include <map>
#include <iostream>
using namespace std;
struct  mylist
{
	string content;
	string type;
	int line;
};
struct errorlist
{
	string error_name;
	int error_line;


};
#endif