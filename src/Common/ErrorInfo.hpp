#pragma once

#include <string>

namespace pl {
    struct ErrorInfo {
        std::string context;
        std::string msg;
        int line;
    };
}