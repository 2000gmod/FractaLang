#pragma once
#include "Common/ErrorInfo.hpp"
#include <vector>
#include "Token.hpp"
#include <filesystem>
#include <istream>
#include <memory>
#include <string_view>

namespace pl {
    void TestPunctuation();

    class Scanner {
        private:
            bool handleValid = false;
            std::shared_ptr<std::istream> inputStream;
            int currentLine = 0;
            std::vector<ErrorInfo> errors;
        
        public:
            ~Scanner() = default;
            static Scanner FromFile(const std::filesystem::path& filepath);
            static Scanner FromString(std::string_view str);
            [[nodiscard]] bool IsOpen() const { return handleValid; }

        private:
            explicit Scanner(const std::filesystem::path& filepath);
            explicit Scanner(std::string_view str);

            void ScannerError(std::string_view msg);

            char Advance();
            char Peek();
            char PeekNext();
            bool Match(char c);

            void ScanToken(Token& out);

            void ScanKeywordOrIdentifier(Token& out, char initial);
            void ScanNumberLiteral(Token& out, char initial);
            void ScanStringLiteral(Token& out);

        public:
            Token GetToken();
            
    };
}