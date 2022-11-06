#pragma once
#include "Option.h"

namespace algos::config {
using EqNullsType = bool;
using NullDIstInfType = bool;
using ParameterType = long double;

template <typename T>
struct OptionInfo {
    OptionInfo(std::string name, std::string description, std::optional<T> default_value = {},
               std::function<void(T &)> value_check = {}) : name_(std::move(name)),
    description_(std::move(description)), default_value_(std::move(default_value)), value_check_(std::move(value_check))
    {}

    OptionInfo(std::string name, std::string description, std::function<void(T &)> value_check = {})
    : name_(std::move(name)), description_(std::move(description)), default_value_({}),
    value_check_(std::move(value_check)) {}

    std::shared_ptr<Option<T>> GetOption(std::function<void(T &)> set,
                                         std::function<void(T const &)> post_set = {}) const {
        assert(set);
        if (value_check_) {
            return std::make_shared<Option<T>>(name_, description_,
                    [this, set](auto value) { value_check_(value); set(value); }, default_value_, post_set);
        }
        return std::make_shared<Option<T>>(name_, description_, set, default_value_, post_set);
    }

    std::optional<T> const default_value_;
    std::string const name_;
    std::string const description_;
    std::function<void(T&)> const value_check_;
};

const OptionInfo<EqNullsType> EqualNulls{opt_strings::kEqualNulls, descriptions::kDEqualNulls, true};
const OptionInfo<NullDIstInfType> NullDistInf{opt_strings::kDistToNullIsInfinity, descriptions::kDDistToNullIsInfinity, false};
const OptionInfo<ParameterType> Parameter{opt_strings::kParameter, descriptions::kDParameter, 0, [](auto value) {
    if (!(value >= 0 && value <= 1)) throw std::invalid_argument("Out of range");
}};

}