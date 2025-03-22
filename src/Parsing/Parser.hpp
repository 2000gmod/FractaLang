#pragma once

#include "Scanner.hpp"
#include "Statement.hpp"
#include "Token.hpp"
#include <filesystem>
#include <string_view>
#include <vector>


namespace pl {
    using SList = std::vector<StmtSP>;

    class FileParser {
        private:
            SList statements;
            std::vector<Token> tokens;
            int current = 0;
        
        public:
            static FileParser FromString(std::string_view str);
            static FileParser FromFile(const std::filesystem::path& path);

            static FileParser FromScanner(Scanner& scanner);
    };
}