#include "Parsing/Scanner.hpp"
#include <fmt/core.h>

int main() {
    auto testSrc = R"(
    func main() {
        return 0;
    }
    )";
    auto sc = pl::Scanner::FromString(testSrc);

    while (sc.IsOpen()) {
        auto t = sc.GetToken();
        fmt::println("{}", t.ToString());
    }
}