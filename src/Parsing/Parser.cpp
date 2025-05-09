#include "Parser.hpp"
#include "Expression.hpp"
#include "Statement.hpp"
#include "Token.hpp"
#include "Type.hpp"
#include "fmt/core.h"
#include "magic_enum/magic_enum.hpp"
#include "../Utils/Utils.hpp"

using namespace pl;

SourceParser SourceParser::FromString(std::string_view str) {
    auto scanner = Scanner::FromString(str);
    return SourceParser::FromScanner(scanner);
}

SourceParser SourceParser::FromFile(const std::filesystem::path &path) {
    auto scanner = Scanner::FromFile(path);
    return SourceParser::FromScanner(scanner);
}

SourceParser SourceParser::FromScanner(Scanner& scanner) {
    SourceParser parser;
    while (scanner.IsOpen()) {
        auto tok = scanner.GetToken();
        parser.tokens.push_back(tok);
    }
    return parser;
}

SList SourceParser::Parse() {
    SList statements;
    while (!IsAtEnd()) {
        try {
            statements.push_back(Statement());
        }
        catch (ParseError& err) {
            ReportError(err.msg);
        }
    }

    return statements;
}

bool SourceParser::IsAtEnd() {
    return Peek().type == TokenType::EoF;
    
}

Token& SourceParser::Peek() {
    return tokens[current];
}

Token& SourceParser::Previous() {
    return tokens.at(current - 1);
}

Token& SourceParser::Advance() {
    if (!IsAtEnd()) current++;
    return Previous();
}

Token& SourceParser::Consume(TokenType type, std::string_view msg) {
    if (Check(type)) return Advance();
    throw Error(Peek(), msg);
}

bool SourceParser::Check(TokenType type) {
    if (IsAtEnd()) return false;
    return type == Peek().type;
}

bool SourceParser::Match(TokenType type) {
    if (Check(type)) {
        Advance();
        return true;
    }
    return false;
}

bool SourceParser::Match(const std::initializer_list<TokenType>& types) {
    for (auto t : types) {
        if (Match(t)) return true;
    }
    return false;
}

SourceParser::ParseError SourceParser::Error(Token& tok, std::string_view msg) {
    auto tokenName = magic_enum::enum_name(tok.type);
    auto message = fmt::format("Fatal parsing error at line {}, token: ({}): \n{}", tok.lineNumber, tokenName, msg);

    return { message };
}

TypeSP SourceParser::TypeExpr() {
    if (Match(TokenType::Identifier)) return TNamed();
    throw Error(Peek(), "Invalid type expression.");
}

TypeSP SourceParser::TNamed() {
    auto name = Previous();
    return MakeSP<NamedType>(name);
}

StmtSP SourceParser::Statement() {
    if (Match(TokenType::KwFunc)) return SFunctionDecl();
    if (Match(TokenType::KwReturn)) return SReturn();
    if (Match(TokenType::OpenBracket)) return SBlock();

    return nullptr;
}

StmtSP SourceParser::SFunctionDecl() {
    auto name = Consume(TokenType::Identifier, "Expected identifier.");
    Consume(TokenType::OpenParen, "Expected '(' after function identifier.");

    std::vector<FuncDeclStmt::ArgPair> args;

    while (!Match(TokenType::CloseParen)) {
        auto pname = Consume(TokenType::Identifier, "Expected parameter identifier.");
        auto ptype = TypeExpr();

        args.emplace_back(ptype, pname);

        if (Match(TokenType::CloseParen)) break;
        else Consume(TokenType::Comma, "Expected ',' before next parameter declaration.");
    }

    auto rtype = TypeExpr();
    StmtSP body;
    
    if (Match(TokenType::OpenBracket)) {
        body = SBlock();
    }
    else if (Match(TokenType::SemiColon)) {
        body = nullptr;
    }
    else {
        throw Error(Peek(), "Invalid token.");
    }

    return MakeSP<FuncDeclStmt>(name, args, rtype, body);
}

StmtSP SourceParser::SReturn() {
    ExprSP value;

    if (Match(TokenType::SemiColon)) value = nullptr;
    else {
        value = Expression();
        Consume(TokenType::SemiColon, "Expected semicolon.");
    }

    return MakeSP<ReturnStmt>(value);
}

StmtSP SourceParser::SBlock() {
    SList body;
    while (!Check(TokenType::CloseBracket)) {
        body.push_back(Statement());
    }
    Consume(TokenType::CloseBracket, "Expected '}' after a block statement.");
    return MakeSP<BlockStmt>(body);
}

ExprSP SourceParser::Expression() {
    return ExLiteral();
}

ExprSP SourceParser::ExLiteral() {
    if (Match({TokenType::IntLiteral, TokenType::DoubleLiteral, TokenType::StringLiteral})) {
        auto value = Previous();
        return MakeSP<LiteralExpr>(value);
    }

    throw Error(Peek(), "Unexpected token.");
}