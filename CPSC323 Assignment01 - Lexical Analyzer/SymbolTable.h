#pragma once
#include "Global_Includes.h"

class SymbolTable {
public:

    void addEntry(const string& name, int address, const string& type);

    // Get the address of a variable
    int getAddress(const string& name) const;

    // Get the data type of a variable
    string getType(const string& name) const;

private:
    struct Entry {
        string name;
        int address;
        string type;
    };
    vector<Entry> entries;
};

