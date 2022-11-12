#pragma once
#include <utility>

#include "OptionInfo.h"
#include "Option.h"

namespace algos::config {
using EqNullsType = bool;
using NullDIstInfType = bool;
using ParameterType = long double;

template <typename T>
struct OptionType {
    explicit OptionType(OptionInfo const info, std::optional<T> default_value = {},
               std::function<void(T &)> value_check = {}) : info_(info),
               default_value_(std::move(default_value)), value_check_(std::move(value_check))
    {}

    explicit OptionType(OptionInfo const info, std::function<void(T &)> value_check = {})
    : OptionType(info, {}, value_check) {}

    [[nodiscard]] Option<T> GetOption(T* value_ptr) const {
        return Option<T>(info_, value_ptr, value_check_, default_value_);
    }

    [[nodiscard]] std::string GetName() const {
        return info_.GetName();
    }

    [[nodiscard]] std::string GetDescription() const {
        return info_.GetDescription();
    }

private:
    std::optional<T> const default_value_;
    OptionInfo const info_;
    std::function<void(T&)> const value_check_;
};

template <typename T, typename... Options>
void AddNames(std::vector<std::string>& names, OptionType<T> opt, Options... options) {
    names.template emplace_back(opt.GetName());
    if constexpr (sizeof...(options) != 0) {
        AddNames(names, options...);
    }
}

template <typename T, typename... Options>
std::vector<std::string> GetOptionNames(OptionType<T> opt, Options... options) {
    std::vector<std::string> names{};
    AddNames(names, opt, options...);
    return names;
}

const OptionType<EqNullsType> EqualNulls{GetOptionInfoByName(opt_names::kEqualNulls), true};
const OptionType<NullDIstInfType> NullDistInf{GetOptionInfoByName(opt_names::kDistToNullIsInfinity), false};
const OptionType<ParameterType> Parameter{GetOptionInfoByName(opt_names::kParameter), 0, [](auto value) {
    if (!(value >= 0 && value <= 1)) throw std::invalid_argument("Out of range");
}};

}