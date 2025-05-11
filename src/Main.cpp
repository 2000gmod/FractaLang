#include "Parsing/Parser.hpp"
#include <fmt/core.h>

int main() {
    const auto testSrc = R"(
        *(alpha + 1)(1, 2 + 3)[1, 2];
    )";

    auto parser = pl::SourceParser::FromString(testSrc);
    auto statements = parser.Parse();

}