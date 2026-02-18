#pragma once

#include <unordered_map>
#include <vector>
#include "utils.hpp"


class SymbolTable {
    std::vector<std::unordered_map<std::string, Symbol>> scopes;
public:
    SymbolTable() {
        // Start with a global scope
        scopes.emplace_back();
    }
    void enterScope() {
        scopes.emplace_back();
    }
    void exitScope() {
        if (!scopes.empty()) {
            scopes.pop_back();
        }
    }
    bool declare(const std::string& name, const Symbol& symbol) {
        if (scopes.empty()) return false; // No scope to declare in
        auto& currentScope = scopes.back();
        if (currentScope.find(name) != currentScope.end()) {
            return false; // Symbol already declared in the current scope
        }
        currentScope[name] = symbol;
        return true;
    }
    Symbol* lookup(const std::string& name) {
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) {
                return &found->second;
            }
        }
        return nullptr; // Not found in any scope
    }
};