#pragma once

#include "Parsing/Statement.hpp"
#include <Parsing/Type.hpp>

#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>


namespace pl {
    enum class MutabilityKind {
        Const,
        Variable
    };

    struct VariableSymbol {
        TypeSP type;
        MutabilityKind mutability;
    };

    struct FunctionSymbol {
        TypeSP returnType;
        std::vector<TypeSP> argTypes;
    };

    struct Symbol {
        std::variant<VariableSymbol, FunctionSymbol> symbol;
    };

    struct SymbolTableFlags {
        bool isInsideFunction = false;
        bool isInsideLoop = false;
    };

    struct SymbolTableEntry {
        std::unordered_map<std::string, Symbol> scopeIdentifiers;
        FuncDeclStmtSP enclosingFunction = nullptr;
        SymbolTableFlags flags;
    };

    struct SymbolTable {
        SymbolTable();

        std::vector<SymbolTableEntry> scopes;

        bool Insert(std::string_view name, const Symbol& symbol);
        bool HasSymbolDefined(std::string_view name);

        std::optional<Symbol> GetSymbol(std::string_view name);

        void CreateScope();
        void DropScope();
    };
}