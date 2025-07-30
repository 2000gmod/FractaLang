#include "Parser.hpp"
#include <Utils/Utils.hpp>

using namespace pl;

ExprSP SourceParser::LiteralParser::Parse(SourceParser&, Token tok) const {
    return MakeSP<LiteralExpr>(tok);
}

ExprSP SourceParser::IdentifierParser::Parse(SourceParser&, Token tok) const {
    return MakeSP<IdentifierExpr>(tok);
}

ExprSP SourceParser::GroupingParser::Parse(SourceParser& src, Token) const {
    auto expr = src.ParseExpression(0);
    src.Consume(TokenType::CloseParen, "Expected ')'.");
    return expr;
}

ExprSP SourceParser::PrefixOperatorParser::Parse(SourceParser& src, Token tok) const {
    auto right = src.ParseExpression(rbp);
    return MakeSP<UnaryExpr>(tok, right);
}

float SourceParser::PrefixOperatorParser::Precedence() const {
    return rbp;
}

ExprSP SourceParser::BinaryOperatorParser::Parse(SourceParser& src, ExprSP left, Token tok) const {
    auto right = src.ParseExpression(rbp);
    return MakeSP<BinaryExpr>(tok, left, right);
}

float SourceParser::BinaryOperatorParser::Lbp() const {
    return lbp;
}

ExprSP SourceParser::PostfixOperatorParser::Parse(SourceParser&, ExprSP left, Token tok) const {
    return MakeSP<UnaryExpr>(tok, left);
}

float SourceParser::PostfixOperatorParser::Precedence() const {
    return precedence;
}

ExprSP SourceParser::CallParser::Parse(SourceParser& src, ExprSP left, Token) const {
    std::vector<ExprSP> args;

    if (!src.Check(TokenType::CloseParen)) {
        args.push_back(src.ParseExpression());
        while (src.Check(TokenType::Comma)) {
            src.Consume(TokenType::Comma, "Expected ','.");
            args.push_back(src.ParseExpression());
        }
    }
    src.Consume(TokenType::CloseParen, "Expected ')'.");
    return MakeSP<CallExpr>(left, args);
}

float SourceParser::CallParser::Precedence() const {
    return precedence;
}

ExprSP SourceParser::IndexParser::Parse(SourceParser& src, ExprSP left, Token) const {
    std::vector<ExprSP> args;

    if (!src.Check(TokenType::CloseSquare)) {
        args.push_back(src.ParseExpression());
        while (src.Check(TokenType::Comma)) {
            src.Consume(TokenType::Comma, "Expected ','.");
            args.push_back(src.ParseExpression());
        }
    }
    src.Consume(TokenType::CloseSquare, "Expected ']'.");
    return MakeSP<IndexExpr>(left, args);
}

float SourceParser::IndexParser::Precedence() const {
    return precedence;
}


