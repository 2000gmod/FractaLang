#pragma once

#include "Common/ErrorInfo.hpp"
#include <concepts>
#include <string_view>
#include <memory>
#include <vector>

namespace pl {
    //void ReportError(std::string_view msg, int code = 1);

    void ReportErrors(const std::vector<ErrorInfo>& errors, bool terminate = false);
    void ReportErrors(std::string_view header, const std::vector<ErrorInfo>& errors, bool terminate = false);

    template <class T, class... Args>
    std::shared_ptr<T> MakeSP(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <class T, class... Args>
    std::unique_ptr<T> MakeUP(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <class Derived, class Base>
    requires std::derived_from<Derived, Base>
    std::shared_ptr<Derived> InstanceOf(const std::shared_ptr<Base>& baseptr) {
        return std::dynamic_pointer_cast<Derived>(baseptr);
    }
}