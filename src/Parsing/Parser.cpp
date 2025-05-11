#include "Parser.hpp"
#include "Expression.hpp"
#include "Statement.hpp"
#include "Token.hpp"
#include "Type.hpp"
#include "fmt/core.h"
#include "magic_enum/magic_enum.hpp"
#include "../Utils/Utils.hpp"

using namespace pl;

SourceParser::SourceParser() {
    prefixParsers = {
        {TokenType::IntLiteral, MakeSP<LiteralParser>()},
        {TokenType::DoubleLiteral, MakeSP<LiteralParser>()},
        {TokenType::StringLiteral, MakeSP<LiteralParser>()},
        {TokenType::Identifier, MakeSP<IdentifierParser>()},
        {TokenType::OpenParen, MakeSP<GroupingParser>()},

        {TokenType::Plus, MakeSP<PrefixOperatorParser>(30)},
        {TokenType::Minus, MakeSP<PrefixOperatorParser>(30)},
        {TokenType::Star, MakeSP<PrefixOperatorParser>(40)},
    };

    infixParsers = {
        {TokenType::Plus, MakeSP<BinaryOperatorParser>(10, 11)},
        {TokenType::Minus, MakeSP<BinaryOperatorParser>(10, 11)},
        {TokenType::Star, MakeSP<BinaryOperatorParser>(20, 21)},
        {TokenType::Slash, MakeSP<BinaryOperatorParser>(20, 21)},
        {TokenType::Mod, MakeSP<BinaryOperatorParser>(20, 21)},
    };

    postfixParsers = {
        {TokenType::OpenParen, MakeSP<CallParser>(50)},
        {TokenType::OpenSquare, MakeSP<IndexParser>(50)},
    };
}

SourceParser::~SourceParser() = default;


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

bool SourceParser::Check(const std::initializer_list<TokenType>& types) {
    for (auto t : types) {
        if (Check(t)) return true;
    }
    return false;
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

    return ParseError(message);
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

    return SExpr();
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
        value = ParseExpression();
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

StmtSP SourceParser::SExpr() {
    auto out = MakeSP<ExprStmt>(ParseExpression());
    Consume(TokenType::SemiColon, "Expected semicolon after expression.");
    return out;
}

ExprSP SourceParser::ParseExpression(float minBp) {
    auto tok = Advance();

    if (!prefixParsers.contains(tok.type)) {
        throw Error(Peek(), "Invalid token.");
    }
    auto prefix = prefixParsers.at(tok.type);

    auto left = prefix->Parse(*this, tok);

    while (true) {
        auto nextTok = Peek();

        std::shared_ptr<PostfixParser> postfix;
        if (postfixParsers.contains(nextTok.type)) postfix = postfixParsers.at(nextTok.type);

        if (postfix && postfix->Precedence() >= minBp) {
            auto tok2 = Advance();
            left = postfix->Parse(*this, left, tok2);
            continue;
        }

        std::shared_ptr<InfixParser> infix;
        if (infixParsers.contains(nextTok.type)) infix = infixParsers.at(nextTok.type);

        if (infix && infix->Lbp() >= minBp) {
            auto tok2 = Advance();
            left = infix->Parse(*this, left, tok2);
            continue;
        }
        break;
    }
    return left;
}