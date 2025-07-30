#include "SemanticAnalysis.hpp"
#include "Analysis/SymbolTable.hpp"
#include "fmt/core.h"
#include <Parsing/Statement.hpp>
#include <memory>
#include <vector>

using namespace pl;

bool SemanticAnalyzer::Analyze(const std::vector<FileSourceNodeSP>& files) {
    PopulateGlobalSymbols(files);

    for (auto& file : files) {
        (void) file;
    }
    return errors.empty();
}

const std::vector<ErrorInfo>& SemanticAnalyzer::GetErrors() {
    return errors;
}

void SemanticAnalyzer::PopulateGlobalSymbols(const std::vector<FileSourceNodeSP>& files) {
    for (const auto& file : files) {
        for (const auto& stmt : file->statements) {
            if (auto p = std::dynamic_pointer_cast<FuncDeclStmt>(stmt); p) {
                FunctionSymbol fsym;
                for (auto& arg : p->args) {
                    fsym.argTypes.emplace_back(arg.type);
                }
                fsym.returnType = p->returnType;
                symbolTable.Insert(p->name.identName, Symbol {fsym});
            }
            else {
                errors.push_back({
                    file->filename, 
                    fmt::format("Unsupported global scope statement, only declarations are allowed."), 
                    stmt->line
                });
            }
        }
    }
}