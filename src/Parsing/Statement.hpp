#pragma once

#include "Token.hpp"
#include "Type.hpp"
#include "Expression.hpp"
#include <memory>
#include <vector>

namespace pl {
    struct StmtBase {
        virtual ~StmtBase() = 0;
    };

    inline StmtBase::~StmtBase() { }

    using StmtSP = std::shared_ptr<StmtBase>;

    struct FuncDeclStmt : public StmtBase {
        struct ArgPair {
            TypeSP type;
            Token name;
        };

        Token name;
        TypeSP returnType;
        std::vector<ArgPair> args;
        StmtSP body;
    };

    struct ReturnStmt : public StmtBase {
        ExprSP value;
    };
}