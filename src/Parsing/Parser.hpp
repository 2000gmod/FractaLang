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


namespace pl {
    using SList = std::vector<StmtSP>;

    class SourceParser {
        private:
            struct ParseError {
                std::string msg;
            };

            std::vector<Token> tokens;
            int current = 0;
        
        public:
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
            bool Match(TokenType type);
            bool Match(const std::initializer_list<TokenType>& types);

            ParseError Error(Token& tok, std::string_view msg);

            TypeSP TypeExpr();
            TypeSP TNamed();

            StmtSP Statement();
            StmtSP SFunctionDecl();
            StmtSP SReturn();
            StmtSP SBlock();

            ExprSP Expression();
            ExprSP ExLiteral();
    };
}