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
    explicit OptionType(OptionInfo info, std::optional<T> default_value = {},
               std::function<void(T &)> value_check = {}) : info_(info),
               default_value_(std::move(default_value)), value_check_(std::move(value_check))
    {}

    explicit OptionType(OptionInfo info, std::function<void(T &)> value_check = {})
    : OptionType(info, {}, value_check) {}

    [[nodiscard]] Option<T> GetOption(T* value_ptr) const {
        return Option<T>(info_, value_ptr, value_check_, default_value_);
    }

    std::optional<T> const default_value_;
    OptionInfo const info_;
    std::function<void(T&)> const value_check_;
};

const OptionType<EqNullsType> EqualNulls{
    GetOptionInfo(option_group_names::common, opt_strings::kEqualNulls), true};
const OptionType<NullDIstInfType> NullDistInf{
    GetOptionInfo(option_group_names::common, opt_strings::kDistToNullIsInfinity), false};
const OptionType<ParameterType> Parameter{
    GetOptionInfo(option_group_names::common, opt_strings::kParameter), 0, [](auto value) {
    if (!(value >= 0 && value <= 1)) throw std::invalid_argument("Out of range");
}};

}