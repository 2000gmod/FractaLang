#pragma once

#include "Parsing/Statement.hpp"
#include <Parsing/Type.hpp>

#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>
#include <stack>


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
        std::string scopeName = "";
        std::unordered_map<std::string, Symbol> scopeIdentifiers;
    };

    class SymbolTable {
        private:
            friend struct RAIIScopeGuard;

            std::stack<FuncDeclStmtSP> functionStack;
            SymbolTableFlags flags;
            std::vector<SymbolTableEntry> scopes;
            std::string currentFilename = "";

        public:
            struct FilenameGuard {
                FilenameGuard() = delete;
                FilenameGuard(const FilenameGuard&) = delete;

                SymbolTable& tableref;

                FilenameGuard(SymbolTable& table, std::string_view filename) : tableref(table) {
                    table.currentFilename = filename;
                }

                ~FilenameGuard() {
                    tableref.currentFilename = "FILE";
                }
            };

            SymbolTable(std::string_view moduleName);

            bool Insert(std::string_view name, const Symbol& symbol);
            bool HasSymbolDefined(std::string_view name);

            std::optional<Symbol> GetSymbol(std::string_view name) const;
            SymbolTableEntry& GetCurrentScope();

            std::string GetModuleName() const;
            std::string GetFileName() const;

            void CreateScope();
            void DropScope();

            bool IsOnModuleScope() const;

            FilenameGuard GetFileGuard(std::string_view filename);
    };

    struct RAIIScopeGuard {
        RAIIScopeGuard() = delete;
        RAIIScopeGuard(const RAIIScopeGuard&) = delete;
        RAIIScopeGuard(RAIIScopeGuard&&) = delete;
        RAIIScopeGuard& operator=(const RAIIScopeGuard&) = delete;
        RAIIScopeGuard& operator=(RAIIScopeGuard&&) = delete;

        SymbolTable& table;

        bool pushedFunction = false;

        RAIIScopeGuard(SymbolTable& table) : table(table) {
            this->table.CreateScope();
        }

        RAIIScopeGuard(SymbolTable& table, const FuncDeclStmtSP& function) : RAIIScopeGuard(table) {
            pushedFunction = true;
            table.functionStack.emplace(function);
        }

        ~RAIIScopeGuard() {
            if (pushedFunction) table.functionStack.pop();
            table.DropScope();
        }
    };
}