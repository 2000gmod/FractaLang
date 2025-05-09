#pragma once

#include "ASTNode.hpp"
#include "Token.hpp"
#include <memory>

namespace pl {
    struct ExprBase : public ASTNode {
        virtual ~ExprBase() = 0;
    };

    inline ExprBase::~ExprBase() { }

    using ExprSP = std::shared_ptr<ExprBase>;

    struct LiteralExpr : public ExprBase {
        Token value;

        LiteralExpr(const Token& value) : value(value) { }
    };

    using LiteralSP = std::shared_ptr<LiteralExpr>;
}