#pragma once

#include "ASTNode.hpp"
#include "Token.hpp"
#include <memory>
#include <utility>


namespace pl {
    struct TypeBase : public ASTNode {
        ~TypeBase() override = 0;
    };

    inline TypeBase::~TypeBase() = default;

    using TypeSP = std::shared_ptr<TypeBase>;

    struct NamedType final : public TypeBase {
        Token name;

        explicit NamedType(Token name) : name(std::move(name)) { }
    };

    using NamedTypeSP = std::shared_ptr<NamedType>;
}