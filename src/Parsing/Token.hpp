#pragma once

#include <variant>
#include <string>
#include <cstdint>

namespace pl {
    enum class TokenType : uint8_t {
        None,
        Error,
        EoF,

        IntLiteral,
        DoubleLiteral,
        StringLiteral,

        Identifier,

        Plus,
        Minus,
        Star,
        Slash,
        Mod,

        Assign,

        Eq,
        NotEq,
        LessThan,
        GreaterThan,
        LessEqual,
        GreaterEqual,

        OpenParen,
        CloseParen,
        OpenSquare,
        CloseSquare,
        OpenBracket,
        CloseBracket,

        Dot,
        Colon,
        DoubleColon,
        Comma,

        SemiColon,

        KwFunc,
        KwReturn,
    };

    struct Token {
        std::string identName;
        int lineNumber;
        TokenType type;

        std::variant<
            std::monostate,
            uint8_t,
            uint16_t,
            uint32_t,
            uint64_t,
            int8_t,
            int16_t,
            int32_t,
            int64_t,
            char,
            float,
            double,
            std::string
        > literalValue = std::monostate();

        std::string ToString() const;
    };
}