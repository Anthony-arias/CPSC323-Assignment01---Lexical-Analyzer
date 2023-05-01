#pragma once
#include "Global_Includes.h"

class SymbolTable {
public:

    /* Constructor to initialize the memory address to 5000 */
    SymbolTable();

    /* Checks if an identifier exists in the symbol table */
    bool contains(const string& identifier) const;

    /* Inserts a new identifier into the symbol table with its type */
    void insert(const string& identifier, const string& type);

    /* Prints out all identifiers in the symbol table with their memory addresses and types */
    void print() const;

    /* Returns the memory address of a given identifier */
    int getAddress(const string& identifier) const;

    /* Returns the type of a given identifier */
    string getType(const string& identifier) const;

private:
    struct Entry {
        int memoryAddress;
        string type;
    };

    unordered_map<string, Entry> entries;
    int memoryAddress;
};


