#include "Parsing/Parser.hpp"
#include <fmt/core.h>

int main() {
    auto testSrc = R"(
    func main() {
        return 0;
    }
    )";
    
    auto parser = pl::FileParser::FromString(testSrc);
    
}