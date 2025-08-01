#include "SymbolTable.hpp"
#include <optional>
#include <string_view>

using namespace pl;

SymbolTable::SymbolTable(std::string_view moduleName) {
    scopes.emplace_back();
    scopes.back().scopeName = moduleName;
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

std::optional<Symbol> SymbolTable::GetSymbol(std::string_view name) const {
    for (const auto& scope : scopes) {
        for (auto& [key, val] : scope.scopeIdentifiers) {
            if (key == name) return val;
        }
    }

    return std::nullopt;
}

SymbolTableEntry& SymbolTable::GetCurrentScope() {
    return scopes.back();
}

std::string SymbolTable::GetModuleName() const {
    return scopes.begin()->scopeName;
}

std::string SymbolTable::GetFileName() const {
    return currentFilename;
}

void SymbolTable::CreateScope() {
    scopes.emplace_back();
}

void SymbolTable::DropScope() {
    if (!scopes.empty()) scopes.pop_back();
}

bool SymbolTable::IsOnModuleScope() const {
    return !scopes.back().scopeName.empty();
}

SymbolTable::FilenameGuard SymbolTable::GetFileGuard(std::string_view filename) {
    return SymbolTable::FilenameGuard { *this, filename };
}