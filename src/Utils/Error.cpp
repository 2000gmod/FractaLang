#include "Utils.hpp"

#include "fmt/color.h"

void pl::ReportError(std::string_view msg, int code) {
    fmt::print(fmt::fg(fmt::color::red), "{}\n", msg);
    std::exit(1);
}