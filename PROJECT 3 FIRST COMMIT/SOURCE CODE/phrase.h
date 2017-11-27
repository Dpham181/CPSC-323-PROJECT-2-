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
