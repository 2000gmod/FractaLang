#pragma once

#include "ASTNode.hpp"
#include "Token.hpp"
#include "Type.hpp"
#include "Expression.hpp"
#include <memory>
#include <utility>
#include <vector>

namespace pl {
    struct StmtBase : public ASTNode {
        ~StmtBase() override = 0;
    };

    inline StmtBase::~StmtBase() = default;

    using StmtSP = std::shared_ptr<StmtBase>;

    struct ExprStmt : public StmtBase {
        ExprSP expr;

        explicit ExprStmt(ExprSP expr) : expr(std::move(expr)) { }
    };

    struct FuncDeclStmt final : public StmtBase {
        struct ArgPair {
            TypeSP type;
            Token name;
        };
        using ArgList = std::vector<ArgPair>;

        Token name;
        ArgList args;
        TypeSP returnType;
        StmtSP body;

        FuncDeclStmt(Token name, ArgList args, TypeSP rType, StmtSP body)
            : name(std::move(name)),
            args(std::move(args)),
            returnType(std::move(rType)),
            body(std::move(body))
            { }
    };

    struct ReturnStmt final : public StmtBase {
        ExprSP value;

        explicit ReturnStmt(ExprSP  value) : value(std::move(value)) { }
    };


    struct BlockStmt final : public StmtBase {
        std::vector<StmtSP> body;

        explicit BlockStmt(std::vector<StmtSP> body) : body(std::move(body)) { }
    };
}