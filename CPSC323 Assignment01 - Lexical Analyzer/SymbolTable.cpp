#include "SymbolTable.h"

// Add an entry to the symbol table
void SymbolTable::addEntry(const string& name, int address, const string& type) {
    entries.push_back({ name, address, type });
}

// Get the address of a variable
int SymbolTable::getAddress(const string& name) const {
    for (const auto& entry : entries) {
        if (entry.name == name) {
            return entry.address;
        }
    }
    // Handle error if name is not found
    throw out_of_range("Name not found in symbol table");
}

// Get the data type of a variable
string SymbolTable::getType(const string& name) const {
    for (const auto& entry : entries) {
        if (entry.name == name) {
            return entry.type;
        }
    }
    // Handle error if name is not found
    throw out_of_range("Name not found in symbol table");
}