#pragma once
#include "Parsing/Statement.hpp"
#include <Parsing/ASTNode.hpp>
#include <string_view>
#include <vector>
#include <Analysis/SymbolTable.hpp>
#include <Common/ErrorInfo.hpp>

namespace pl {

    class SemanticAnalyzer {
        private:
            std::vector<FileSourceNodeSP> files;

        public:
            SemanticAnalyzer(const std::vector<FileSourceNodeSP>& files, std::string_view moduleName);
            //~SemanticAnalyzer();

            void Analyze();

            [[nodiscard]] const std::vector<ErrorInfo>& GetErrors() const { return errors; }
            [[nodiscard]] bool HadErrors() const { return !errors.empty(); }


        private:
            SymbolTable symbolTable;

            std::vector<ErrorInfo> errors;

            void AddError(std::string_view msg, int line);

            void PopulateGlobalSymbols(const std::vector<FileSourceNodeSP>& files);

            void AnalyzeFile(FileSourceNodeSP file);
            void AnalyzeStatement(StmtSP stmt);

            void AnalyzeExprStatement(ExprStmtSP exsp);
            void AnalyzeFuncDeclStatement(FuncDeclStmtSP func);
            void AnalyzeReturnStatement(ReturnStmtSP ret);
            void AnalyzeBlockStatement(BlockStmtSP block);
    };
}
