#pragma once
#include <memory>

namespace pl {
    struct ASTNode {
        virtual ~ASTNode() = 0;
    };
    inline ASTNode::~ASTNode() = default;

    using ASTNodeSP = std::shared_ptr<ASTNode>;
}
