#pragma once
#include <utility>

#include "Option.h"

namespace algos::config {
using EqNullsType = bool;
using NullDIstInfType = bool;
using ParameterType = long double;

template <typename T>
struct OptionType {
    OptionType(std::string name, std::string description, std::optional<T> default_value = {},
               std::function<void(T &)> value_check = {}) : name_(std::move(name)),
    description_(std::move(description)), default_value_(std::move(default_value)), value_check_(std::move(value_check))
    {}

    OptionType(std::string name, std::string description, std::function<void(T &)> value_check = {})
    : OptionType(name, description, {}, value_check) {}

    [[nodiscard]] Option<T> GetOption() const {
        return Option<T>(name_, description_, value_check_, default_value_);
    }

    std::optional<T> const default_value_;
    std::string const name_;
    std::string const description_;
    std::function<void(T&)> const value_check_;
};

const OptionType<EqNullsType> EqualNulls{opt_strings::kEqualNulls, descriptions::kDEqualNulls, true};
const OptionType<NullDIstInfType> NullDistInf{opt_strings::kDistToNullIsInfinity, descriptions::kDDistToNullIsInfinity, false};
const OptionType<ParameterType> Parameter{opt_strings::kParameter, descriptions::kDParameter, 0, [](auto value) {
    if (!(value >= 0 && value <= 1)) throw std::invalid_argument("Out of range");
}};

}