#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

struct Symbol {
    string identifier;
    int memorylocation;
    string type;
};

class SymbolTable {
private:
    vector<Symbol> data;
    int memory_address;
public:
    SymbolTable() : memory_address(10000)
    {}

    bool contains(string identifier)
    {
        for (auto s : data) {
            if (s.identifier == identifier) {
                return true;
            }
        }
        return false;
    }

    Symbol get(string identifier)
    {
        for (auto s : data) {
            if (s.identifier == identifier) {
                return s;
            }
        }
        return {"???", -1, "Unknown"};
    }

    bool insert(string identifier, string type)
    {
        if (contains(identifier)) {
            return false;
        } else {
            data.push_back({identifier, memory_address++, type});
            return true;
        }
    }

    int size()
    {
        return data.size();
    }

    Symbol operator[](int i) const
    {
        return data[i];
    }

    void print()
    {
        cout << setw(20) << "Identifier" << setw(20) << "MemoryLocation" << setw(10) << "Type\n";
        for (auto s : data) {
            cout << setw(20) << s.identifier << setw(20) << s.memorylocation << setw(10) << s.type << "\n";
        }
    }
};

#endif
