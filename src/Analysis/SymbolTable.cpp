#include "SymbolTable.hpp"
#include <optional>
#include <string_view>

using namespace pl;

SymbolTable::SymbolTable() {
    scopes.emplace_back();
}

bool SymbolTable::Insert(std::string_view name, const Symbol& symbol) {
    if (HasSymbolDefined(name)) return false;

    scopes.back().scopeIdentifiers.insert(std::pair {name, symbol});

    return true;
}

bool SymbolTable::HasSymbolDefined(std::string_view name) {
    for (const auto& entry : scopes) {
        if (entry.scopeIdentifiers.contains(std::string(name))) return true;
    }
    return false;
}

std::optional<Symbol> SymbolTable::GetSymbol(std::string_view name) {
    for (const auto& scope : scopes) {
        for (auto& [key, val] : scope.scopeIdentifiers) {
            if (key == name) return val;
        }
    }

    return std::nullopt;
}

void SymbolTable::CreateScope() {
    scopes.emplace_back();
}

void SymbolTable::DropScope() {
    if (!scopes.empty()) scopes.pop_back();
}