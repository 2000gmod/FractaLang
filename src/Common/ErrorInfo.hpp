#pragma once

#include <string>

namespace pl {
    struct ErrorInfo {
        std::string filename;
        std::string msg;
        int line;
    };
}