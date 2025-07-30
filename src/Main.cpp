#include "Analysis/SemanticAnalysis.hpp"
#include <Parsing/Parser.hpp>
#include <fmt/core.h>
#include <fmt/color.h>


const auto testSource = R"(
return 2;

func Main() i32 {
    return 2;
}
)";

int main() {
    auto parser = pl::SourceParser::FromString(testSource, "main_testsrc.fr");
    auto node = parser.Parse();

    pl::SemanticAnalyzer sema;
    if (!sema.Analyze({node})) {
        auto errors = sema.GetErrors();

        for (auto& error : errors) {
            fmt::print(fmt::fg(fmt::color::red), "Error [{}, line {}]: {}\n", error.filename, error.line, error.msg);
        }
    }
}