#pragma once

#include "Token.hpp"
#include <memory>


namespace pl {
    struct TypeBase {
        virtual ~TypeBase() = 0;
    };

    inline TypeBase::~TypeBase() { }

    using TypeSP = std::shared_ptr<TypeBase>;

    struct NamedType : public TypeBase {
        Token name;
    };
}