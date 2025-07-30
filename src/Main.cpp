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
)";

int main() {
    auto parser = pl::SourceParser::FromString(testSource, "main_testsrc.fr");
    auto node = parser.Parse();

    if (parser.HadErrors()) {
        pl::ReportErrors("Parsing errors:", parser.GetErrors());
    }

    pl::SemanticAnalyzer sema;
    if (!sema.Analyze({node})) {
        pl::ReportErrors("Semantic analysis errors:", sema.GetErrors());
    }
}