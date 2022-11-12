#pragma once
#include <any>
#include <string>
#include <memory>

namespace algos::config {
struct IOption {
    virtual void SetDefault() = 0;
    virtual void SetAny(std::any value) = 0;
    virtual void Unset() = 0;
    [[nodiscard]] virtual bool IsSet() const = 0;
    [[nodiscard]] virtual std::string GetName() const = 0;
    virtual ~IOption() = default;
};
}

