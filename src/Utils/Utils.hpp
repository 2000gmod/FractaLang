#pragma once

#include <string_view>
#include <memory>

namespace pl {
    void ReportError(std::string_view msg, int code = 1);

    template <class T, class... Args>
    std::shared_ptr<T> MakeSP(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <class T, class... Args>
    std::unique_ptr<T> MakeUP(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}