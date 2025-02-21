#include "Scanner.hpp"
#include "Token.hpp"
#include "fmt/core.h"
#include "magic_enum/magic_enum.hpp"
#include "../Utils/Utils.hpp"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace pl;

static const std::unordered_map<std::string, TokenType> Keywords = {
    {"func", TokenType::KwFunc},
    {"return", TokenType::KwReturn},
};

static const std::unordered_map<std::string, TokenType> Punctuations = {
    {"+", TokenType::Plus},
    {"-", TokenType::Minus},
    {"*", TokenType::Star},
    {"/", TokenType::Slash},
    {"%", TokenType::Mod},

    {"=", TokenType::Assign},

    {"==", TokenType::Eq},
    {"!=", TokenType::NotEq},
    {"<", TokenType::LessThan},
    {">", TokenType::GreaterThan},
    {"<=", TokenType::LessEqual},
    {">=", TokenType::GreaterEqual},

    {"(", TokenType::OpenParen},
    {")", TokenType::CloseParen},
    {"[", TokenType::OpenSquare},
    {"]", TokenType::CloseSquare},
    {"{", TokenType::OpenBracket},
    {"}", TokenType::CloseBracket},

    {".", TokenType::Dot},
    {":", TokenType::Colon},
    {"::", TokenType::DoubleColon},
    {";", TokenType::SemiColon},
};

static std::vector<std::string> SortedPunctuations = [] {
    std::vector<std::string> keys;

    for (const auto& [key, val] : Punctuations) {
        keys.push_back(key);
    }
    std::sort(keys.begin(), keys.end());
    return keys;
}();

enum class MatchResult : uint8_t {
    None,
    Partial,
    MatchButLongerPossible,
    FullMatch,
};

static MatchResult MatchPunctuation(const std::string& str) {
    using enum MatchResult;

    auto it = std::lower_bound(SortedPunctuations.begin(), SortedPunctuations.end(), str);

    bool exactMatch = (it != SortedPunctuations.end() && *it == str);
    bool isPrefix = false;

    for (const auto& token : SortedPunctuations) {
        if (token.find(str) == 0) {
            isPrefix = true;
            if (token != str && exactMatch) {
                return MatchButLongerPossible;
            }
        }
    }
    if (isPrefix && !exactMatch) return Partial;
    if (exactMatch) return FullMatch;
    if (isPrefix) return MatchButLongerPossible;
    return None;
}

void pl::TestPunctuation() {
    MatchPunctuation("=");
    std::string str;
    
    while (true) {
        std::cout << ">> ";
        std::cin >> str;
        auto res = MatchPunctuation(str);
        auto name = magic_enum::enum_name(res);

        fmt::println("{}", name);
    }
}

void pl::ScannerError(std::string_view msg) {
    ReportError(fmt::format("[Scanner error] {}", msg));
}

static bool IsAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool IsNumeric(char c) {
    return (c >= '0' && c <= '9');
}

static bool IsAlphaNum(char c) {
    bool out = 
        IsAlpha(c) ||
        IsNumeric(c) ||
        c == '_' ||
        c == '$';

    return out;
}

Scanner Scanner::FromFile(const std::filesystem::path &filepath) {
    return Scanner(filepath);
}

Scanner Scanner::FromString(std::string_view str) {
    return Scanner(str);
}

Scanner::Scanner(const std::filesystem::path& filepath) {
    HandleValid = std::filesystem::exists(filepath);
    if(HandleValid) {
        auto ptr = std::make_shared<std::ifstream>();
        ptr->open(filepath.string());
        InputStream = ptr;
    }
}

Scanner::Scanner(std::string_view str) {
    HandleValid = !str.empty();

    if (HandleValid) {
        auto ptr = std::make_shared<std::stringstream>();
        ptr->str(std::string(str));
        InputStream = ptr;
    }
}

Token Scanner::GetToken() {
    Token out;

    if (!HandleValid) {
        out.Type = TokenType::Error;
    }
    else if (InputStream->eof()) {
        HandleValid = false;
        out.Type = TokenType::EoF;
    }
    else {
        ScanToken(out);
    }

    return out;
}

char Scanner::Advance() {
    char c = InputStream->get();
    if (InputStream->eof()) {
        HandleValid = false;
        return '\0';
    }
    return c;
}

char Scanner::Peek() {
    if (InputStream->eof()) {
        HandleValid = false;
        return '\0';
    }
    return InputStream->peek();
}

char Scanner::PeekNext() {
    char c = Advance();
    InputStream->unget();
    return c;
}

bool Scanner::Match(char c) {
    if (Peek() == c) {
        Advance();
        return true;
    }
    return false;
}

void Scanner::ScanToken(Token& out) {
    char c = Advance();
    if (InputStream->eof()) {
        out.Type = TokenType::EoF;
        return;
    }

    { // Skip conditions
        bool skipFlag = false;
        do {
            switch (c) {
                case '#':
                    skipFlag = true;
                    while (c != '\n' && HandleValid) c = Advance();
                    continue;
                case '\n':
                    CurrentLine++;
                case ' ':
                case '\t':
                case '\r':
                    skipFlag = true;
                    break;
                default:
                    skipFlag = false;
                    break;
            }
            if(skipFlag) c = Advance();
        } while (skipFlag);
    }

    if (InputStream->eof()) {
        out.Type = TokenType::EoF;
        return;
    }

    { // Punctuations
        std::string proc { c };
        std::string biggestMatch;

        auto res = MatchPunctuation(proc);

        if (res == MatchResult::None) goto L1;

        if (res == MatchResult::MatchButLongerPossible) {
            biggestMatch = proc;
        }
        
        else if (res == MatchResult::FullMatch) {
            out.Type = Punctuations.at(proc);
            out.LineNumber = CurrentLine;
            return;
        }

        while (true) {
            proc += Advance();
            res = MatchPunctuation(proc);

            switch (res) {
                case MatchResult::None: {
                    if (!Punctuations.contains(biggestMatch)) {
                        out.LineNumber = CurrentLine;
                        out.Type = TokenType::Error;
                        return;
                    }
                    out.Type = Punctuations.at(biggestMatch);
                    out.LineNumber = CurrentLine;
                    return;
                }
                case MatchResult::Partial: {
                    continue;
                }
                case MatchResult::MatchButLongerPossible: {
                    biggestMatch = proc;
                    continue;
                }
                case MatchResult::FullMatch: {
                    out.Type = Punctuations.at(proc);
                    out.LineNumber = CurrentLine;
                    return;
                }
            }
        }
    }
    
    L1:

    if (IsAlpha(c)) return ScanKeywordOrIdentifier(out, c);
    if (IsNumeric(c)) return ScanNumberLiteral(out, c);
    if (c == '"') return ScanStringLiteral(out);
}

void Scanner::ScanKeywordOrIdentifier(Token& out, char initial) {
    out.LineNumber = CurrentLine;
    std::string proc { initial };

    while (IsAlphaNum(Peek())) proc += Advance();

    if (Keywords.contains(proc)) {
        out.Type = Keywords.at(proc);
        return;
    }
    
    
    out.Type = TokenType::Identifier;
    out.IdentName = proc;
    return;
}

void Scanner::ScanNumberLiteral(Token& out, char initial) {
    out.LineNumber = CurrentLine;
    std::string proc { initial };

    while (IsNumeric(Peek())) proc += Advance();

    if (Peek() == '.') {
        Advance();
        proc += '.';
        while (IsNumeric(Peek())) proc += Advance();

        out.Type = TokenType::DoubleLiteral;
        auto val = std::stod(proc);
        out.LiteralValue = val;
        return;
    }

    
    try {
        out.Type = TokenType::IntLiteral;
        auto val = std::stoll(proc);
        out.LiteralValue = val;
    }
    catch (std::out_of_range&) {
        out.Type = TokenType::Error;
        ScannerError(fmt::format("(at line {}) Integer literal out of max range: ({})", CurrentLine, proc));
        return;
    }
    return;
}

void Scanner::ScanStringLiteral(Token& out) {
    out.LineNumber = CurrentLine;
    auto startLine = CurrentLine;

    char previous = '"';

    char c;
    std::string proc;
    bool terminated = false;

    while (HandleValid) {
        c = Advance();
        if (c == '"' && previous != '\\') {
            terminated = true;
            break;
        }
        if (c == '\\') {
            c = Advance();

            switch (c) {
                case 'n':
                    proc += '\n';
                    break;
                case 't':
                    proc += '\t';
                    break;
                case 'r':
                    proc += '\r';
                    break;
                
                case '"':
                    proc += '"';
                    break;
                case '\\':
                    proc += '\\';
                    break;
            }
            //Advance();
            continue;
        }
        if (c == '\n') {
            CurrentLine++;
            continue;
        }
        else proc += c;
        previous = c;
    }
    
    out.Type = terminated ? TokenType::StringLiteral : TokenType::Error;
    if (terminated) out.LiteralValue = proc;
    else {
        ScannerError(fmt::format("Unterminated string literal at line {}", startLine));
    }
}