#pragma once

#include "ASTNode.hpp"
#include "Token.hpp"
#include <memory>


namespace pl {
    struct TypeBase : public ASTNode {
        virtual ~TypeBase() = 0;
    };

    inline TypeBase::~TypeBase() { }

    using TypeSP = std::shared_ptr<TypeBase>;

    struct NamedType : public TypeBase {
        Token name;

        NamedType(const Token& name) : name(name) { }
    };

    using NamedTypeSP = std::shared_ptr<NamedType>;
}