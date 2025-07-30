#include "Token.hpp"
#include "fmt/core.h"
#include "magic_enum/magic_enum.hpp"

std::string pl::Token::ToString() const {
    auto tname = magic_enum::enum_name(type);

    if (tname.ends_with("Literal")) {
        const auto litType = tname.substr(0, tname.size() - 7);

        if (litType == "Int") {
            return fmt::format("{}: {}", tname, std::get<int64_t>(literalValue));
        }
        if (litType == "Double") {
            return fmt::format("{}: {}", tname, std::get<double>(literalValue));
        }
        if (litType == "String") {
            return fmt::format("{}: \"{}\"", tname, std::get<std::string>(literalValue));
        }
    }
    if (tname == "Identifier") {
        return fmt::format("{}: \"{}\"", tname, identName);
    }
    return fmt::format("{}", tname);
}

bool pl::Token::Check(TokenType t) const {
    return (t == type);
}

bool pl::Token::Check(const std::initializer_list<TokenType> types) const {
    for (const auto& t : types) {
        if (Check(t)) return true;
    }
    return false;
}

bool pl::Token::IsLiteral() const {
    switch (type) {
        case TokenType::IntLiteral:
        case TokenType::DoubleLiteral:
        case TokenType::StringLiteral:
            return true;
        default:
            return false;
    }
}
