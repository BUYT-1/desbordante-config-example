#pragma once

#include "ProgramOptionStrings.h"
#include "Descriptions.h"
#include "Option.h"

namespace algos::config {

template <typename PT>
struct OptNullEqualsNull : DefOption<bool, PT, &program_option_strings::kEqualNulls,
        &algos::config::descriptions::kDEqualNulls> {
    OptNullEqualsNull() : DefOption<bool, PT, &program_option_strings::kEqualNulls,
    &algos::config::descriptions::kDEqualNulls>(true) {}
};

}
