#pragma once
#include <typeindex>
#include <unordered_map>
#include <concepts>

#include "ASTNode.hpp"

namespace pl {
    class SemanticAnalyzer {
        public:
            SemanticAnalyzer();
            ~SemanticAnalyzer();

            void Analyze(const std::vector<FileSourceNodeSP>& files);

        private:
            struct SemanticContext {
                
            };

            class IAnalyzer {
                public:
                    virtual ~IAnalyzer() = default;
                    virtual void Analyze(SemanticAnalyzer& analyzer, SemanticContext& context, const ASTNodeSP& node) = 0;
            };

            template <class T>
            requires std::derived_from<T, ASTNode>
            class AnalyzerBase : public IAnalyzer {
                public:
                    void Analyze(SemanticAnalyzer &analyzer, SemanticContext &context, const ASTNodeSP &node) final {
                        auto real = std::dynamic_pointer_cast<T>(node);
                        if (!real) return;
                        RunAnalysis(analyzer, context, real);
                    }
                protected:
                    virtual void RunAnalysis(SemanticAnalyzer &analyzer, SemanticContext &context, const std::shared_ptr<T> &node) = 0;
            };

            std::unordered_map<std::type_index, std::vector<std::unique_ptr<IAnalyzer>>> analyzerRegistry;

            template <class NodeType, class PassType, class... Args>
            void Register(Args&& ...args) {
                analyzerRegistry[typeid(NodeType)].push_back(MakeUP<PassType>(std::forward(args)...));
            }

            SemanticContext moduleContext;
    };
}
