#include "Common/ErrorInfo.hpp"
#include "Utils.hpp"

#include "fmt/color.h"

/*void pl::ReportError(const std::string_view msg, int code) {
    fmt::print(fmt::fg(fmt::color::red), "{}\n", msg);
    std::exit(code);
}*/

void pl::ReportErrors(const std::vector<ErrorInfo>& errors, bool terminate) {
   return ReportErrors("", errors, terminate);
}

void pl::ReportErrors(std::string_view header, const std::vector<ErrorInfo>& errors, bool terminate) {
    if (errors.empty()) return;
    if (!header.empty()) fmt::print(fmt::emphasis::bold, "{}\n", header);
    for (const auto& error : errors) {
        fmt::print(fmt::fg(fmt::color::red), "Error [at {}, line {}]:\n\t{}\n", error.context, error.line, error.msg);
    }

    if (terminate) std::exit(1);
}