#pragma once

namespace pl {
    struct ASTNode {
        virtual ~ASTNode() = 0;
    };
    inline ASTNode::~ASTNode() = default;
}