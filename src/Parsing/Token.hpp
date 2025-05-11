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

        [[nodiscard]] std::string ToString() const;

        Token(Token&& other) noexcept :
            identName(std::move(other.identName)),
            lineNumber(other.lineNumber),
            type(other.type),
            literalValue(std::move(other.literalValue))
        { }

        Token(const Token& other) :
            identName(other.identName),
            lineNumber(other.lineNumber),
            type(other.type),
            literalValue(other.literalValue)
        { }

        Token() = default;

        [[nodiscard]] bool Check(TokenType t) const;

        [[nodiscard]] bool Check(std::initializer_list<TokenType> types) const;

        [[nodiscard]] bool IsLiteral() const;
    };
}