#include "Analysis/SemanticAnalysis.hpp"
#include "Utils/Utils.hpp"
#include <Parsing/Parser.hpp>
#include <fmt/core.h>
#include <fmt/color.h>


const auto testSource = R"(
return 2;

func Main() i32 {
    return 2
}

func aaa(Main b) void {
    return 2;
}
)";

int main() {
    auto parser = pl::SourceParser::FromString(testSource, "test.fr");
    auto node = parser.Parse();

    if (parser.HadErrors()) {
        pl::ReportErrors("Parsing errors.", parser.GetErrors());
    }

    pl::SemanticAnalyzer sema({node}, "TestModule");
    sema.Analyze();
    if (sema.HadErrors()) {
        pl::ReportErrors("Semantic analysis errors.", sema.GetErrors());
    }
}