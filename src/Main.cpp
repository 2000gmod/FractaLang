#include "Parsing/Parser.hpp"
#include <fmt/core.h>

int main() {
    auto testSrc = R"(
    func main() void {
        return;
    }
    )";
    
    auto parser = pl::SourceParser::FromString(testSrc);
    auto statements = parser.Parse();
}