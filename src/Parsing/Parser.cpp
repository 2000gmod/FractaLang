#include "Parser.hpp"

using namespace pl;

FileParser FileParser::FromString(std::string_view str) {
    auto scanner = Scanner::FromString(str);
    return FileParser::FromScanner(scanner);
}

FileParser FileParser::FromFile(const std::filesystem::path &path) {
    auto scanner = Scanner::FromFile(path);
    return FileParser::FromScanner(scanner);
}

FileParser FileParser::FromScanner(Scanner& scanner) {
    FileParser parser;
    while (scanner.IsOpen()) {
        auto tok = scanner.GetToken();
        parser.tokens.push_back(tok);
    }
    return parser;
}