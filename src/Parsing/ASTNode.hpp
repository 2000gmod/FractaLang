#pragma once
#include <memory>
#include <vector>

namespace pl {
    struct ASTNode {
        virtual ~ASTNode() = 0;
    };
    inline ASTNode::~ASTNode() = default;

    using ASTNodeSP = std::shared_ptr<ASTNode>;

    struct StmtBase;

    struct FileSourceNode : public ASTNode {
        std::string filename;
        std::vector<std::shared_ptr<StmtBase>> statements;

        FileSourceNode(const std::string& filename, std::vector<std::shared_ptr<StmtBase>> statements)
            : filename(filename), 
              statements(std::move(statements)) 
            { }
    };

    using FileSourceNodeSP = std::shared_ptr<FileSourceNode>;
}
