#pragma once

#include "Expression.hpp"
#include "Scanner.hpp"
#include "Statement.hpp"
#include "Token.hpp"
#include "Type.hpp"
#include <filesystem>
#include <initializer_list>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <unordered_map>

namespace pl {
    class ExpressionParser;
    using SList = std::vector<StmtSP>;

    class SourceParser {
        private:
            struct ParseError final : std::exception {
                std::string msg;
                explicit ParseError(std::string msg) : msg(std::move(msg)) { }
            };

            std::vector<Token> tokens;
            int current = 0;

            SourceParser();
        public:

            ~SourceParser();
            static SourceParser FromString(std::string_view str);
            static SourceParser FromFile(const std::filesystem::path& path);

            static SourceParser FromScanner(Scanner& scanner);

            SList Parse();

        private:
            bool IsAtEnd();
            Token& Peek();
            Token& Previous();
            Token& Advance();
            Token& Consume(TokenType type, std::string_view msg);
            bool Check(TokenType type);
            bool Check(const std::initializer_list<TokenType>& types);
            bool Match(TokenType type);
            bool Match(const std::initializer_list<TokenType>& types);

            static ParseError Error(Token& tok, std::string_view msg);

            TypeSP TypeExpr();
            TypeSP TNamed();

            StmtSP Statement();
            StmtSP SFunctionDecl();
            StmtSP SReturn();
            StmtSP SBlock();

            StmtSP SExpr();

            ExprSP ParseExpression(float minBp = 0.0f);



            struct PrefixParser {
                virtual ~PrefixParser() = default;

                [[nodiscard]] virtual ExprSP Parse(SourceParser&, Token) const = 0;
                [[nodiscard]] virtual float Precedence() const { return 0.0f; }
            };

            struct InfixParser {
                virtual ~InfixParser() = default;

                [[nodiscard]] virtual ExprSP Parse(SourceParser&, ExprSP left, Token) const = 0;
                [[nodiscard]] virtual float Lbp() const { return 0.0f; }
            };

            struct PostfixParser {
                virtual ~PostfixParser() = default;

                [[nodiscard]] virtual ExprSP Parse(SourceParser&, ExprSP left, Token) const = 0;
                [[nodiscard]] virtual float Precedence() const { return 0.0f; }
            };



            struct LiteralParser final : public PrefixParser {
                [[nodiscard]] ExprSP Parse(SourceParser&, Token) const override;
            };

            struct IdentifierParser final : public PrefixParser {
                [[nodiscard]] ExprSP Parse(SourceParser&, Token) const override;
            };

            struct GroupingParser final : public PrefixParser {
                [[nodiscard]] ExprSP Parse(SourceParser&, Token) const override;
            };

            struct PrefixOperatorParser final : public PrefixParser {
                float rbp;
                explicit PrefixOperatorParser(const float rbp) : rbp(rbp) { }

                [[nodiscard]] ExprSP Parse(SourceParser&, Token) const override;
                [[nodiscard]] float Precedence() const override;
            };

            struct BinaryOperatorParser final : public InfixParser {
                float lbp, rbp;
                BinaryOperatorParser(float lb, float rbp) : lbp(lb), rbp(rbp) { }

                [[nodiscard]] ExprSP Parse(SourceParser&, ExprSP left, Token) const override;
                [[nodiscard]] float Lbp() const override;
            };

            struct PostfixOperatorParser final : public PostfixParser {
                float precedence;
                explicit PostfixOperatorParser(float precedence) : precedence(precedence) { }

                [[nodiscard]] ExprSP Parse(SourceParser &, ExprSP left, Token) const override;
                [[nodiscard]] float Precedence() const override;
            };

            struct CallParser final : public PostfixParser {
                float precedence;
                explicit CallParser(float precedence) : precedence(precedence) { }

                [[nodiscard]] ExprSP Parse(SourceParser&, ExprSP left, Token) const override;
                [[nodiscard]] float Precedence() const override;
            };

            struct IndexParser final : public PostfixParser {
                float precedence;
                explicit IndexParser(float precedence) : precedence(precedence) { }

                [[nodiscard]] ExprSP Parse(SourceParser&, ExprSP left, Token) const override;
                [[nodiscard]] float Precedence() const override;
            };

            std::unordered_map<TokenType, std::shared_ptr<PrefixParser>> prefixParsers;
            std::unordered_map<TokenType, std::shared_ptr<InfixParser>> infixParsers;
            std::unordered_map<TokenType, std::shared_ptr<PostfixParser>> postfixParsers;
    };
}
