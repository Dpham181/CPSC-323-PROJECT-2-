#ifndef PARSE_H
#define PARSE_H

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <windows.h>
#include "phrase.h"
#include "symbol_table.h"

using namespace std;

class Alloperation
{
public:
    void start();
    void add(mylist n);
    SymbolTable symboltable;
    vector<string> asminstructions;
private:
    void check_length();
    void check_type_mismatch(string expected, string actual);
    Symbol check_symbol_is_declared(string lexeme);
    Symbol check_symbol_is_declared(string lexeme, string type);
    void declare_symbol(string lexeme, string type);
    void display_errors();
    void start_point();
    void Opt_Declaration_List();
    void Statement_List();
    void Statement_List1();
    void Body();
    void Write_statement();
    void Read_statement();
    void Return_statement();
    void Return_statement1();
    void Assign_statement();
    void if_statement();
    void while_statement();
    void expression_func(string expected_type = "");
    void Term(string expected_type);
    void IDs();
    void Factor(string expected_type);
    void primary1(string expected_type);
    void condition();
    void invalid_id();
    int asm_emit_inst(string inst);
    int asm_emit_inst(string inst, int value);
    int asm_emit_inst(string inst, string arg);
    void asm_emit_operator(char op);
    void asm_emit_operator(string op);
    void operator_process(string op);
    //all variables
    errorlist error_object;
    vector<mylist> alltokens;
    vector<errorlist> allerrors;
    int token_lenth;
    int update_index;
    int inserted_index;
    vector<string> allcontent;
    vector<string> alltypes;
    vector<int> lineofcode;
    stack<char> operatorstack;
    bool stop;
};

void Alloperation::add(mylist n)
{
    alltokens.push_back(n);
}

//if there is another tokens to parse
void Alloperation::check_length()
{
    if (update_index < inserted_index - 1) {
        update_index++;
    } else {
        stop = true;

        if (allerrors.size() == 0) {
            cout << "...................................." << endl;
            cout << "No syntax error" << endl;
        }
    }
}

Symbol Alloperation::check_symbol_is_declared(string lexeme)
{
    if (!symboltable.contains(lexeme)) {
        error_object.error_name = "undeclared symbol: " + lexeme;
        error_object.error_line = lineofcode[update_index];
        allerrors.push_back(error_object);
        update_index = inserted_index - 1;
        display_errors();
    }
    return symboltable.get(lexeme);
}

void Alloperation::check_type_mismatch(string expected, string actual)
{
    if (expected != "" && expected != actual) {
        error_object.error_name = "type mismatch, expected " + expected + ", actual is " + actual;
        error_object.error_line = lineofcode[update_index];
        allerrors.push_back(error_object);
        update_index = inserted_index - 1;
        display_errors();
    }
}

Symbol Alloperation::check_symbol_is_declared(string lexeme, string type)
{
    Symbol s = check_symbol_is_declared(lexeme);
    if (s.memorylocation < 0) {
        return s;
    }
    check_type_mismatch(type, s.type);
    return s;
}

void Alloperation::declare_symbol(string lexeme, string type)
{
    if (!symboltable.insert(lexeme, type)) {
        error_object.error_name = "redeclared symbol: " + lexeme;
        error_object.error_line = lineofcode[update_index];
        allerrors.push_back(error_object);
        update_index = inserted_index - 1;
        display_errors();
    }
}

void Alloperation::invalid_id()
{
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
    for (vector<errorlist>::iterator it = allerrors.begin(); it != allerrors.end(); ++it) {
        cout << "Error is:" << (*it).error_name << "\t" << "line:" << (*it).error_line << endl;
    }
}

/*
Emit assembly instruction, returns number so it can be used as label if needed
*/
int Alloperation::asm_emit_inst(string inst)
{
    asminstructions.push_back(inst);
    return asminstructions.size();
}

int Alloperation::asm_emit_inst(string inst, int value)
{
    return asm_emit_inst(inst + " " + to_string(value));
}

int Alloperation::asm_emit_inst(string inst, string arg)
{
    return asm_emit_inst(inst + " " + arg);
}

void Alloperation::asm_emit_operator(char op)
{
    switch (op) {
    case '+':
        asm_emit_inst("ADD");
        break;
    case '-':
        asm_emit_inst("SUB");
        break;
    case '*':
        asm_emit_inst("MUL");
        break;
    case '/':
        asm_emit_inst("DIV");
        break;
    case '>':
        asm_emit_inst("GRT");
        break;
    case '<':
        asm_emit_inst("LES");
        break;
    case '=':
        asm_emit_inst("EQU");
        break;
    default:
        asm_emit_inst("???");
    }
}

void Alloperation::asm_emit_operator(string sop)
{
    if (sop.size() == 1) {
        char op = sop[0];
        asm_emit_operator(op);
    } else if (sop == "<=") {
        // turn  a <= b  
        asm_emit_inst("LEQ");
    } else if (sop == "=>") {
        // turn  a >= b  
        asm_emit_inst("GEQ");
    } else if (sop == "/=") {
        // turn  a != b 
        asm_emit_inst("NEQ");
    } else {
        asm_emit_inst("???");
    }
}

void Alloperation::operator_process(string sop)
{
    if (sop.size() == 0) {
        while (!operatorstack.empty()) {
            asm_emit_operator(operatorstack.top());
            operatorstack.pop();
        }
    } else {
        char op = sop[0];
        if (operatorstack.empty() || op == '(') {
            operatorstack.push(op);
        } else if (op == ')') {
            char current;
            while ((current = operatorstack.top()) != '(') {
                operatorstack.pop();
                asm_emit_operator(current);
            }
            operatorstack.pop();
        } else if (op == '*' || op == '/') {
            operatorstack.push(op);
        } else {
            asm_emit_operator(operatorstack.top());
            operatorstack.pop();
            operator_process(sop);
        }
    }
}

void Alloperation::start()
{
    stop = false;

    token_lenth = alltokens.size();
    update_index = 0;
    inserted_index = 0;

    allcontent.resize(alltokens.size());
    alltypes.resize(alltokens.size());
    lineofcode.resize(alltokens.size());
    for (vector<mylist>::iterator it = alltokens.begin(); it != alltokens.end(); ++it) {
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
    stop = static_cast<int>(allcontent.size()) <= update_index;
    if (!stop && allcontent[update_index] == "%%") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Function Definition'> <Epsilon>" << endl;
        cout << "\n\n";
        check_length();
        Opt_Declaration_List();
        Statement_List();

        asm_emit_inst("LABEL"); // always emit a label at the end in case there is a jump here
    } else {
        if (!stop) {
            error_object.error_name = "missed %%";
            error_object.error_line = lineofcode[update_index];
            allerrors.push_back(error_object);
            update_index = inserted_index - 1;
            display_errors();
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
<Qualifier> ::= integer   |  boolean
<IDs> ::=  <Identifier>    | <Identifier>, <IDs>
to remove left recursion  IDs ::= identifier IDs'
IDs'::=,identifier|empty

*/
void Alloperation::Opt_Declaration_List()
{
    if (allcontent[update_index] == "integer" || allcontent[update_index] == "boolean") {
        string symboltype = allcontent[update_index];
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
        if (alltypes[update_index] == "identifier") {
            cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
            cout << endl;
            cout << "\t" << "<Opt Declaration List> <Declaration List> " << endl;
            cout << "\t" << " <Declartion list> <Declaration>; <Declaration list'>" << endl;
            cout << "\t" << " <Declaration> <Qualifier><ID>" << endl;
            cout << "\t" << "<IDs> <identifier> <IDs'>" << endl;
            cout << "\n\n";

            declare_symbol(allcontent[update_index], symboltype);

            check_length();
            if (allcontent[update_index] == ",") {
                cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
                cout << endl;
                cout << "\t" << "<Opt Declaration List> <Declaration List> " << endl;
                cout << "\t" << " <Declartion list> <Declaration>; <Declaration list'>" << endl;
                cout << "\t" << " <Declaration> <Qualifier><ID>" << endl;
                cout << "\t" << "<IDs> <identifier> <IDs'>" << endl;
                cout << "\t" << "<IDs'><,identifier>" << endl;
                cout << "\n\n";

                check_length();
                if (alltypes[update_index] == "identifier") {
                    goto first;
                } else {
                    error_object.error_name = "missed identifier";
                    error_object.error_line = lineofcode[update_index];
                    allerrors.push_back(error_object);
                    update_index = inserted_index - 1;
                    display_errors();
                }
            } else if (allcontent[update_index] == ";") {
                cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
                cout << "\t" << "<IDs'> -> Epsilon" << endl;
                cout << "\n\n";
                check_length();
                if (allcontent[update_index] == "integer" || allcontent[update_index] == "boolean") {
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

            else {
                error_object.error_name = "must be new identifier with , or ;";
                error_object.error_line = lineofcode[update_index];
                allerrors.push_back(error_object);
                update_index = inserted_index - 1;
                display_errors();
            }
        } else {
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
    if (allcontent[update_index] == "{") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Opt Declaration List><Epsilon>" << endl;
        cout << "\t" << "<Body> { <Statement List> }" << endl;
        cout << "\n\n";

        check_length();
    //first:
        Statement_List();
        if (allcontent[update_index] == "}") {
            cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
            cout << "\t" << "<Statement List'>  <Epsilon>" << endl;
            cout << "\n\n";

            check_length();
        } else {
            if (!stop) {
                error_object.error_name = "missed } for body";
                error_object.error_line = lineofcode[update_index];
                allerrors.push_back(error_object);
                update_index = inserted_index - 1;
                display_errors();
            }
        }
    } else {
        if (!stop) {
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
    if (allcontent[update_index] == "write") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Statement> <write>" << endl;
        cout << "\t" << "<Write> write (<Expression>);" << endl;
        cout << "\n\n";

        check_length();
        Write_statement();
        Statement_List1();
    } else if (allcontent[update_index] == "read") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Statement> <read>" << endl;
        check_length();
        Read_statement();
        Statement_List1();
    } else if (allcontent[update_index] == "if") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Statement> <if>" << endl;
        cout << "\t" << "<If> if (<Condition>) <Statement> <If >" << endl;
        cout << "\n\n";
        check_length();
        if_statement();
        Statement_List1();
    } else if (allcontent[update_index] == "while") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Statement> <While>" << endl;
        cout << "\n\n";

        check_length();
        while_statement();
        Statement_List1();
    } else if (allcontent[update_index] == "return") {
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
    else if (allcontent[update_index] == "{") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Statement> {<compound> }" << endl;
        cout << "\t" << "<Compound> { <StatementList> }" << endl;

        cout << "\n\n";

        check_length();
        Statement_List1();
        if (allcontent[update_index] == "}") {
            cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
            cout << "\t" << "<Statement List'>  <Epsilon>" << endl;

            cout << "\n\n";

            check_length();
        } else {
            if (!stop) {
                error_object.error_name = "missed } for compound";
                error_object.error_line = lineofcode[update_index];
                allerrors.push_back(error_object);
                update_index = inserted_index - 1;
                display_errors();
            }
        }
    } else if (alltypes[update_index] == "identifier") {
        Assign_statement();
        Statement_List1();
    } else {
        if (!stop&&allcontent[update_index] != "}") {
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
    if (allcontent[update_index] == "write") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Statement List> <Statement> <Statement List'>" << endl;
        cout << "\t" << "<Statement List'> <Statement>" << endl;
        cout << "\t" << "<Statement> <write>" << endl;
        cout << "\t" << "<Write> write(<Expression>);" << endl;
        cout << "\n\n";

        check_length();
        Write_statement();
        Statement_List();
    } else if (allcontent[update_index] == "read") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Statement List> <Statement> <Statement List'>" << endl;
        cout << "\t" << "<Statement List'> <Statement>" << endl;
        cout << "\t" << "<Statement> <read>" << endl;
        cout << "\n\n";

        check_length();
        Read_statement();
        Statement_List();
    } else if (allcontent[update_index] == "if") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Statement List> <Statement> <Statement List'>" << endl;
        cout << "\t" << "<Statement List'> <Statement>" << endl;
        cout << "\t" << "<Statement> <if>" << endl;
        cout << "\t" << "<If> if (<Condition>) <Statement> <If'>" << endl;
        cout << "\n\n";

        check_length();
        if_statement();
        Statement_List();
    } else if (allcontent[update_index] == "while") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Statement List> <Statement> <Statement List'>" << endl;
        cout << "\t" << "<Statement List'> <Statement>" << endl;
        cout << "\t" << "<Statement> <while>" << endl;
        cout << "\n\n";

        check_length();
        while_statement();
        Statement_List();
    } else if (allcontent[update_index] == "return") {
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
    else if (allcontent[update_index] == "{") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Statement List> <Statement> <Statement List'>" << endl;
        cout << "\t" << "<Statement List'> <Statement>" << endl;
        cout << "\t" << "<Statement> {<compound> }" << endl;
        cout << "\n\n";

        check_length();
        Statement_List();
        if (allcontent[update_index] == "}") {
            cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
            cout << "\t" << "<Statement List'>  <Epsilon>" << endl;

            check_length();
            Statement_List();
        } else {
            if (!stop) {
                error_object.error_name = "missed } for compound";
                error_object.error_line = lineofcode[update_index];
                allerrors.push_back(error_object);
                update_index = inserted_index - 1;
                display_errors();
            }
        }
    } else if (alltypes[update_index] == "identifier") {
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
    if (allcontent[update_index] == "(") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        check_length();
        invalid_id();

    first:
        if (alltypes[update_index] == "identifier") {
            cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
            cout << "\t" << "<Statement> <read> " << endl;
            cout << "\t" << " <read> (<IDs>)" << endl;
            cout << "\t" << "<IDs> <identifier> <IDs'>" << endl;
            cout << "\n\n";

            asm_emit_inst("STDIN");
            asm_emit_inst("POPM", check_symbol_is_declared(allcontent[update_index], "integer").memorylocation);

            check_length();

        //second:
            if (allcontent[update_index] == ",") {
                cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
                cout << "\t" << " <read> (<IDs>)" << endl;
                cout << "\t" << "<IDs> <identifier> <IDs'>" << endl;
                cout << "\t" << "<IDs'> ,<identifier> " << endl;
                cout << "\n\n";

                check_length();
                if (alltypes[update_index] == "identifier") {
                    goto first;
                } else {
                    if (!stop) {
                        error_object.error_name = "missed identifier after ,";
                        error_object.error_line = lineofcode[update_index];
                        allerrors.push_back(error_object);
                        update_index = inserted_index - 1;
                        display_errors();
                        // goto second;
                    }
                }
            }
            if (allcontent[update_index] == ")") {
                cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
                cout << "\t" << "<ID'> <Epsilon>" << endl;

                cout << "\n\n";

                check_length();
            //third:
                if (allcontent[update_index] == ";") {
                    cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
                    cout << "\n\n";

                    check_length();
                } else {
                    if (!stop) {
                        error_object.error_name = "missed ;";
                        error_object.error_line = lineofcode[update_index];
                        allerrors.push_back(error_object);
                        update_index = inserted_index - 1;
                        display_errors();
                    }
                }
            } else {
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
        } else {
            if (!stop) {
                error_object.error_name = "missed ( for read ";
                error_object.error_line = lineofcode[update_index];
                allerrors.push_back(error_object);
                update_index = inserted_index - 1;
                display_errors();
                // goto second;
            }
        }
    } else {
        if (!stop) {
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
    if (allcontent[update_index] == "(") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\n\n";
        invalid_id();

        check_length();
    //first:
        expression_func();

        operator_process("");

        asm_emit_inst("STDOUT");

        Statement_List1();
        if (allcontent[update_index] == ")") {
            cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
            cout << "\t" << "<Primary'> <Epsilon> " << endl;
            cout << "\t" << "<Term'> <Epsilon> " << endl;
            cout << "\t" << "<Expression'> <Epsilon> " << endl;
            cout << "\n\n";

            check_length();
        //second:
            if (allcontent[update_index] == ";") {
                cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;

                cout << "\n\n";

                check_length();
            } else {
                if (!stop) {
                    error_object.error_name = "missed ; ";
                    error_object.error_line = lineofcode[update_index];
                    allerrors.push_back(error_object);
                    update_index = inserted_index - 1;
                    display_errors();
                }
            }
        } else {
            if (!stop) {
                error_object.error_name = "missed ) for write ";
                error_object.error_line = lineofcode[update_index];
                allerrors.push_back(error_object);
                update_index = inserted_index - 1;
                display_errors();
                //goto second;
            }
        }
    } else {
        if (!stop) {
            error_object.error_name = "missed ( for write ";
            error_object.error_line = lineofcode[update_index];
            allerrors.push_back(error_object);
            update_index = inserted_index - 1;
            display_errors();
            //goto first;
        }
    }
}

void  Alloperation::Return_statement1()
{
    if (alltypes[update_index] == "integer") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Return'> <Expression>;" << endl;
        cout << "\t" << "<Expression> <Term> <Expression'>" << endl;
        cout << "\t" << "<Term> <Factor> <Term'>" << endl;
        cout << "\t" << "<Factor> <Primary>" << endl;
        cout << "\t" << "<Primary> <integer>" << endl;
        cout << "\n\n";

        asm_emit_inst("PUSHI", allcontent[update_index]);

        check_length();
    }
}

void Alloperation::Return_statement()
{
    if (allcontent[update_index] == ";") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Statement> <Return>" << endl;
        cout << "\t" << "<Return> <return> <;>" << endl;
        cout << "\n\n";
        check_length();
    } else {
        expression_func();

        operator_process("");

        if (allcontent[update_index] == ";") {
            cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
            cout << "\t" << "<Primary'> <Epsilon>" << endl;
            cout << "\t" << "<Term'> <Epsilon>" << endl;
            cout << "\t" << "<Expression'> <Epsilon>" << endl;
            cout << "\n\n";
            check_length();
        }

        else {
            if (!stop) {
                error_object.error_name = "missed ; ";
                error_object.error_line = lineofcode[update_index];
                allerrors.push_back(error_object);
            }
        }
    }
}

void Alloperation::while_statement()
{
    int labelbegin = asm_emit_inst("LABEL");

    if (allcontent[update_index] == "(") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\n\n";
        invalid_id();

        check_length();
    //first:
        condition();

        if (allcontent[update_index] == ")") {
            cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
            cout << "\t" << "<Primary'> <Epsilon>" << endl;
            cout << "\t" << "<Term'> <Epsilon>" << endl;
            cout << "\t" << "<Expression'> <Epsilon>" << endl;
            cout << "\n\n";

            int jumpend = asm_emit_inst("JUMPZ ");

            check_length();
        //second:
            Statement_List();

            int end = asm_emit_inst("JUMP", labelbegin);
            asminstructions[jumpend - 1] += to_string(end + 1); // fix up for jumpz of condition
        } else {
            if (!stop) {
                error_object.error_name = "missed ( for write ";
                error_object.error_line = lineofcode[update_index];
                allerrors.push_back(error_object);
                update_index = inserted_index - 1;
                display_errors();
                //goto second;
            }
        }
    } else {
        if (!stop) {
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
    if (allcontent[update_index] == "(") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\n\n";
        invalid_id();

        check_length();
    //first:
        condition();

        if (allcontent[update_index] == ")") {
            cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
            cout << "\t" << "<Term'> <Epsilon>" << endl;
            cout << "\t" << "<Expression'> <Epsilon>" << endl;
            cout << "\n\n";

            int jumpcond = asm_emit_inst("JUMPZ ");

            check_length();
        //second:
            Statement_List();
            if (allcontent[update_index] == "fi") {
                cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
                cout << "\t" << "<If'> < fi > " << endl;
                cout << "\n\n";

                asminstructions[jumpcond - 1] += to_string(asminstructions.size() + 1); // fix up for jumpz of condition

                check_length();
            } else if (allcontent[update_index] == "else") {
                cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
                cout << "\t" << "<Statement> <if>" << endl;
                cout << "\t" << "<If'> -> else <Statement> fi" << endl;
                cout << "\n\n";

                int jumpelse = asm_emit_inst("JUMP ");
                asminstructions[jumpcond - 1] += to_string(jumpelse + 1); // fix up for jumpz of condition

                check_length();

            //third:
                Statement_List();
                if (allcontent[update_index] == "fi") {
                    cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
                    cout << "\n\n";

                    asminstructions[jumpelse - 1] += to_string(asminstructions.size() + 1); // fix up for jump of else

                    check_length();
                } else {
                    if (!stop) {
                        error_object.error_name = "missed fi for else statement ";
                        error_object.error_line = lineofcode[update_index];
                        allerrors.push_back(error_object);
                        update_index = inserted_index - 1;
                        display_errors();
                    }
                }
            } else {
                if (!stop) {
                    error_object.error_name = "missed second part of if_statement ";
                    error_object.error_line = lineofcode[update_index];
                    allerrors.push_back(error_object);
                    update_index = inserted_index - 1;
                    display_errors();
                    // goto first;
                }
            }
        } else {
            if (!stop) {
                error_object.error_name = "missed ) for condition ";
                error_object.error_line = lineofcode[update_index];
                allerrors.push_back(error_object);
                update_index = inserted_index - 1;
                display_errors();
                // goto second;
            }
        }
    } else {
        if (!stop) {
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
    cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
    cout << "\t" << "<Statement> <Assign>" << endl;
    cout << "\t" << "<Asign> <identifier>" << endl;
    cout << "\n\n";

    Symbol symbol = check_symbol_is_declared(allcontent[update_index]);

    check_length();

    if (allcontent[update_index] == ":=") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\n\n";
        invalid_id();

        check_length();
    //first:
        expression_func(symbol.type);

        operator_process("");

        if (allcontent[update_index] == ";") {
            cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
            cout << "\t" << "<Primary'> <Epsilon>" << endl;
            cout << "\t" << "<Term'> <Epsilon>" << endl;
            cout << "\t" << "<Expression'> <Epsilon>" << endl;

            cout << "\n\n";

            asm_emit_inst("POPM", symbol.memorylocation);

            check_length();
        } else {
            if (!stop) {
                error_object.error_name = "missed ; ";
                error_object.error_line = lineofcode[update_index];
                allerrors.push_back(error_object);
                update_index = inserted_index - 1;
                display_errors();
            }
        }
    } else {
        if (!stop) {
            error_object.error_name = "missed equal operator for assign";
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
void Alloperation::expression_func(string expected_type)
{
    Term(expected_type);
first:
    if (allcontent[update_index] == "+") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Expression> <Term><Expression'>" << endl;
        cout << "\t" << "<Expression'> +<Term><Expression'>" << endl;
        cout << "\n\n";

        operator_process(allcontent[update_index]);

        check_length();
        Term(expected_type);
        goto first;
    } else if (allcontent[update_index] == "-") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Expression> <Term><Expression'>" << endl;
        cout << "\t" << "<Expression'> -<Term><Expression'>" << endl;
        cout << "\n\n";

        operator_process(allcontent[update_index]);

        check_length();

        Term(expected_type);
        goto first;
    }
}

/*
this method for Term
<Term>    ::=  <Term> * <Factor>  | <Term> / <Factor> |  <Factor>
<Term> ::=<Factor><Term'>
<Term'>::=/<Factor><Term'>|*<Factor><Term'>|empty

*/
void Alloperation::Term(string expected_type)
{
    Factor(expected_type);
first:
    if (allcontent[update_index] == "*") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Term> <Factor> <Term'>" << endl;
        cout << "\t" << "<Term'> *<Factor> <Term'>" << endl;
        cout << "\n\n";

        operator_process(allcontent[update_index]);

        check_length();
        Factor(expected_type);
        goto first;
    } else if (allcontent[update_index] == "/") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Term> <Factor> <Term'>" << endl;
        cout << "\t" << "<Term'> /<Factor> <Term'>" << endl;
        cout << "\n\n";

        operator_process(allcontent[update_index]);

        check_length();

        Factor(expected_type);
        goto first;
    }
}

/*
this method  for factor
<Factor> ::= - <Primary>   | <Primary>
*/
void Alloperation::Factor(string expected_type)
{
    if (allcontent[update_index] == "-") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Factor> -<Primary>" << endl;
        check_length();

        asm_emit_inst("PUSHI", 0);

        primary1(expected_type);

        asm_emit_inst("SUB");
    } else {
        primary1(expected_type);
    }
}

/*
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> ) | true | false
*/
void Alloperation::primary1(string expected_type)
{
    invalid_id();

    if (alltypes[update_index] == "identifier") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Expression> <Term> <Expression'>" << endl;
        cout << "\t" << "<Term> <Factor> <Term'>" << endl;
        cout << "\t" << "<Factor> <Primary>" << endl;
        cout << "\t" << "<Primary> <identifier><Primary'>" << endl;
        cout << "\n\n";

        Symbol symbol = check_symbol_is_declared(allcontent[update_index], expected_type);
        asm_emit_inst("PUSHM", symbol.memorylocation);

        check_length();
        if (allcontent[update_index] == "[") {
            cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
            cout << "\t" << "<Primary'> [<IDs>]" << endl;
            cout << "\n\n";

            check_length();
            IDs();

            if (allcontent[update_index] == "]") {
                cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
                cout << "\t" << "<IDs'> <Epsilon>" << endl;

                cout << "\n\n";

                check_length();
            } else {
                if (!stop) {
                    error_object.error_name = "missed ]";
                    error_object.error_line = lineofcode[update_index];
                    allerrors.push_back(error_object);
                    update_index = inserted_index - 1;
                    display_errors();
                }
            }
        }
    } else if (alltypes[update_index] == "integer") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Factor> <Primary>" << endl;
        cout << "\t" << "<Primary> <integer>" << endl;
        cout << "\n\n";

        check_type_mismatch(expected_type, "integer");

        asm_emit_inst("PUSHI", allcontent[update_index]);

        check_length();
    } else if (allcontent[update_index] == "(") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Factor> <Primary>" << endl;
        cout << "\t" << "<Primary> (<Expression>)" << endl;
        cout << "\n\n";

        operator_process(allcontent[update_index]);

        update_index++;
        expression_func();
        if (allcontent[update_index] == ")") {
            cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
            cout << "\t" << "<Term'> <Epsilon>" << endl;
            cout << "\t" << "<Expression'> <Epsilon>" << endl;
            cout << "\n\n";

            operator_process(allcontent[update_index]);

            check_length();
        } else {
            if (!stop) {
                error_object.error_name = "missed )";
                error_object.error_line = lineofcode[update_index];
                allerrors.push_back(error_object);
            }
        }
    } else if (allcontent[update_index] == ";") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
    } else if (allcontent[update_index] == "true") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Expression> <Term> <Expression'>" << endl;
        cout << "\t" << "<Term> <Factor> <Term'>" << endl;
        cout << "\t" << "<Factor> <Primary>" << endl;
        cout << "\t" << "<Primary> <true>" << endl;
        cout << "\n\n";

        check_type_mismatch(expected_type, "boolean");

        asm_emit_inst("PUSHI", 1);

        check_length();
    } else if (allcontent[update_index] == "false") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Expression> <Term> <Expression'>" << endl;
        cout << "\t" << "<Term> <Factor> <Term'>" << endl;
        cout << "\t" << "<Factor> <Primary>" << endl;
        cout << "\t" << "<Primary> <false>" << endl;
        cout << "\n\n";

        check_type_mismatch(expected_type, "boolean");

        asm_emit_inst("PUSHI", 0);

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

    if (alltypes[update_index] == "identifier") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<IDs> <identifier> <IDs'>" << endl;
        cout << "\n\n";

        check_length();
    first:
        if (allcontent[update_index] == ",") {
            cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
            cout << "\t" << "<IDs> <identifier> <IDs'>" << endl;
            cout << "\t" << "<IDs'><,identifier>" << endl;
            check_length();
            if (alltypes[update_index] == "identifier") {
                goto first;
            } else {
                if (!stop) {
                    error_object.error_name = "missed identifier after ,";
                    error_object.error_line = lineofcode[update_index];
                    allerrors.push_back(error_object);
                    update_index = inserted_index - 1;
                    display_errors();
                }
            }
        }
    } else {
        if (!stop) {
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
    string op = "";

    expression_func();
    if (allcontent[update_index] == ":=") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        check_length();
    } else if (allcontent[update_index] == "=" || allcontent[update_index] == ">" || allcontent[update_index] == "<" || allcontent[update_index] == "/=" || allcontent[update_index] == "=>" || allcontent[update_index] == "<=") {
        cout << "Token:" << alltypes[update_index] << "\t" << "Lexeme:" << allcontent[update_index] << endl;
        cout << "\t" << "<Primary '> <Epsilon>" << endl;
        cout << "\t" << "<Term '> <Epsilon>" << endl;
        cout << "\t" << "<Expression'> <Epsilon>" << endl;
        cout << "\t" << "<Relop> < " << allcontent[update_index] << " >" << endl;
        cout << "\n\n";

        op = allcontent[update_index];

        check_length();
    } else {
        if (!stop) {
            error_object.error_name = "missed Relop  ";
            error_object.error_line = lineofcode[update_index];
            allerrors.push_back(error_object);
            update_index = inserted_index - 1;
            display_errors();
        }
    }
    expression_func();

    asm_emit_operator(op);

    operator_process("");
}

#endif
