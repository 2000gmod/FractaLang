#pragma once

#include "ASTNode.hpp"
#include "Token.hpp"
#include <memory>
#include <utility>
#include <vector>

namespace pl {
    struct TypeBase;
    using TypeSP = std::shared_ptr<TypeBase>;

    struct ExprBase : public ASTNode {
        ~ExprBase() override = 0;
        TypeSP exprType;
        int line = 0;
    };

    inline ExprBase::~ExprBase() = default;

    using ExprSP = std::shared_ptr<ExprBase>;

    struct LiteralExpr final : public ExprBase {
        Token value;

        explicit LiteralExpr(Token value) : value(std::move(value)) { }
    };

    using LiteralSP = std::shared_ptr<LiteralExpr>;

    struct IdentifierExpr final : public ExprBase {
        Token value;

        explicit IdentifierExpr(Token value) : value(std::move(value)) { }
    };

    using IdentifierSP = std::shared_ptr<IdentifierExpr>;

    struct UnaryExpr final : public ExprBase {
        Token op;
        ExprSP subExpr;

        explicit UnaryExpr(Token op, ExprSP subExpr) : op(std::move(op)), subExpr(std::move(subExpr)) { }
    };

    using UnarySP = std::shared_ptr<UnaryExpr>;

    struct BinaryExpr final : public ExprBase {
        Token op;
        ExprSP left, right;

        explicit BinaryExpr(Token op, ExprSP left, ExprSP right) : op(std::move(op)), left(std::move(left)), right(std::move(right)) { }
    };

    using BinarySP = std::shared_ptr<BinaryExpr>;

    struct ParenExpr final : public ExprBase {
        ExprSP subExpr;

        explicit ParenExpr(ExprSP subExpr) : subExpr(std::move(subExpr)) { }
    };

    using ParenSP = std::shared_ptr<ParenExpr>;


    struct CallExpr final : public ExprBase {
        ExprSP callee;
        std::vector<ExprSP> args;

        explicit CallExpr(ExprSP callee, std::vector<ExprSP> args) : callee(std::move(callee)), args(std::move(args)) { }
    };

    using CallSP = std::shared_ptr<CallExpr>;

    struct IndexExpr final : public ExprBase {
        ExprSP indexedExpr;
        std::vector<ExprSP> indices;

        explicit IndexExpr(ExprSP indexedExpr, std::vector<ExprSP> indices) : indexedExpr(std::move(indexedExpr)), indices(std::move(indices)) { }
    };

    using IndexSP = std::shared_ptr<IndexExpr>;
}