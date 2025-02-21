#include "Token.hpp"
#include "fmt/core.h"
#include "magic_enum/magic_enum.hpp"

std::string pl::Token::ToString() const {
    auto tname = magic_enum::enum_name(Type);

    if (tname.starts_with("Kw")) {
        return fmt::format("{}", tname);
    }
    if (tname.ends_with("Literal")) {
        auto litType = tname.substr(0, tname.size() - 7);

        if (litType == "Int") {
            return fmt::format("{}: {}", tname, std::get<int64_t>(LiteralValue));
        }
        if (litType == "Double") {
            return fmt::format("{}: {}", tname, std::get<double>(LiteralValue));
        }
        if (litType == "String") {
            return fmt::format("{}: \"{}\"", tname, std::get<std::string>(LiteralValue));
        }
    }
    if (tname == "Identifier") {
        return fmt::format("{}: \"{}\"", tname, IdentName);
    }
    return fmt::format("{}", tname);
}