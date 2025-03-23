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
        using ArgList = std::vector<ArgPair>;

        Token name;
        ArgList args;
        TypeSP returnType;
        StmtSP body;

        FuncDeclStmt(const Token& name, const ArgList& args, const TypeSP& rType, const StmtSP& body) 
            : name(name),
            args(args),
            returnType(rType),
            body(body)
            { }
    };

    struct ReturnStmt : public StmtBase {
        ExprSP value;

        ReturnStmt(const ExprSP& value) : value(value) { }
    };


    struct BlockStmt : public StmtBase {
        std::vector<StmtSP> body;

        BlockStmt(const std::vector<StmtSP>& body) : body(body) { }
    };
}