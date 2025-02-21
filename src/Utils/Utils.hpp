#pragma once

#include <string_view>
namespace pl {
    void ReportError(std::string_view msg, int code = 1);
}