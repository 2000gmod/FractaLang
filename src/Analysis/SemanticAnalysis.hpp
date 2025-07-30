#pragma once
#include <Parsing/ASTNode.hpp>
#include <vector>
#include <Analysis/SymbolTable.hpp>
#include <Common/ErrorInfo.hpp>

namespace pl {

    class SemanticAnalyzer {
        public:
            //SemanticAnalyzer();
            //~SemanticAnalyzer();

            bool Analyze(const std::vector<FileSourceNodeSP>& files);
            const std::vector<ErrorInfo>& GetErrors();

        private:
            SymbolTable symbolTable;

            std::vector<ErrorInfo> errors;

            void PopulateGlobalSymbols(const std::vector<FileSourceNodeSP>& files);
    };
}
