#pragma once

#include <stdexcept>
#include "Option.h"
#include "Descriptions.h"

namespace algos::config {

template <typename PT>
struct OptParameter : Option<long double, PT, &program_option_strings::kParameter,
        &algos::config::descriptions::kDParameter> {
    OptParameter() : Option<long double, PT, &program_option_strings::kParameter,
    &algos::config::descriptions::kDParameter>(0) {}

    void Set(PT& primitive, long double value) override {
        if (!(value >= 0 && value <= 1)) throw std::exception();
        Option<long double, PT, &program_option_strings::kParameter,
        &algos::config::descriptions::kDParameter>::Set(primitive, value);
    }
};

}