#include "SymbolTable.h"

SymbolTable::SymbolTable() : memoryAddress(5000) {}

bool SymbolTable::contains(const string& identifier) const {
    return entries.find(identifier) != entries.end();
}

void SymbolTable::insert(const string& identifier, const string& type) {
    if (contains(identifier)) {
        throw runtime_error("Error: identifier already exists in symbol table");
    }
    entries[identifier] = { memoryAddress++, type };
}

void SymbolTable::print() const {
    cout << "Identifier\tMemory Address\tType\n";
    for (const auto& entry : entries) {
        cout << entry.first << "\t\t" << entry.second.memoryAddress << "\t\t" << entry.second.type << "\n";
    }
}

int SymbolTable::getAddress(const string& identifier) const {
    if (!contains(identifier)) {
        throw runtime_error("Error: identifier not found in symbol table");
    }
    return entries.at(identifier).memoryAddress;
}

string SymbolTable::getType(const string& identifier) const {
    if (!contains(identifier)) {
        throw runtime_error("Error: identifier not found in symbol table");
    }
    return entries.at(identifier).type;
}

void SymbolTable::update(const string& identifier, const string& type) {
    if (!contains(identifier)) {
        throw runtime_error("Error: identifier not found in symbol table");
    }
    entries[identifier].type = type;
}

