#include "SemanticAnalysis.hpp"
#include "Analysis/SymbolTable.hpp"
#include "fmt/core.h"
#include <Parsing/Statement.hpp>
#include <Utils/Utils.hpp>
#include <string_view>
#include <vector>

using namespace pl;

SemanticAnalyzer::SemanticAnalyzer(const std::vector<FileSourceNodeSP>& files, std::string_view moduleName)
    : files(files), symbolTable(moduleName) {
    PopulateGlobalSymbols(files);
}

void SemanticAnalyzer::Analyze() {
    for (auto& file : files) {
        AnalyzeFile(file);
    }
}

void SemanticAnalyzer::AddError(std::string_view msg, int line) {
    errors.push_back({
        fmt::format("{}:{}", symbolTable.GetModuleName(), symbolTable.GetFileName()),
        std::string(msg),
        line
    });
}

void SemanticAnalyzer::PopulateGlobalSymbols(const std::vector<FileSourceNodeSP>& files) {
    for (const auto& file : files) {
        auto fileguard = symbolTable.GetFileGuard(file->filename);
        for (const auto& stmt : file->statements) {
            if (auto p = InstanceOf<FuncDeclStmt>(stmt)) {
                FunctionSymbol fsym;
                for (auto& arg : p->args) {
                    fsym.argTypes.emplace_back(arg.type);
                }
                fsym.returnType = p->returnType;
                auto success = symbolTable.Insert(p->name.identName, Symbol {fsym});
                if (!success) {
                    AddError(fmt::format("Function '{}' already defined.", p->name.identName), p->line);
                }
            }
            else {
                AddError(
                    "Global scope only supports function and type declarations.",
                    stmt->line
                );
            }
        }
    }
}

void SemanticAnalyzer::AnalyzeFile(FileSourceNodeSP file) {
    auto fileguard = symbolTable.GetFileGuard(file->filename);
    for (auto& stmt : file->statements) {
        AnalyzeStatement(stmt);
    }
}

void SemanticAnalyzer::AnalyzeStatement(StmtSP stmt) {
    if (auto p = InstanceOf<ExprStmt>(stmt)) {
        AnalyzeExprStatement(p);
    }
    else if (auto p = InstanceOf<FuncDeclStmt>(stmt)) {
        AnalyzeFuncDeclStatement(p);
    }
    else if (auto p = InstanceOf<ReturnStmt>(stmt)) {
        AnalyzeReturnStatement(p);
    }
    else if (auto p = InstanceOf<BlockStmt>(stmt)) {
        AnalyzeBlockStatement(p);
    }
    else {
        AddError("Unsupported statement type.", stmt->line);
    }
}

void SemanticAnalyzer::AnalyzeExprStatement(ExprStmtSP exsp) {
    (void) exsp;
}

void SemanticAnalyzer::AnalyzeFuncDeclStatement(FuncDeclStmtSP func) {
    if (!symbolTable.IsOnModuleScope()) {
        AddError("Functions may only be declared on file scope.", func->line);
        return;
    }
    RAIIScopeGuard guard(symbolTable, func);

    for (auto& arg : func->args) {
        auto success = symbolTable.Insert(
            arg.name.identName,
            {.symbol = VariableSymbol {
                arg.type, MutabilityKind::Variable
            }
        });

        if (!success) {
            AddError(fmt::format("Argument '{}' shadows already existing name.", arg.name.identName), func->line);
            return;
        }
    }

    AnalyzeStatement(func->body);
}

void SemanticAnalyzer::AnalyzeReturnStatement(ReturnStmtSP ret) {
    (void) ret;
}

void SemanticAnalyzer::AnalyzeBlockStatement(BlockStmtSP block) {
    (void) block;
}