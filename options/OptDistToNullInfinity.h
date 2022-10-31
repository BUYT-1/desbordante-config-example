#pragma once

#include "ProgramOptionStrings.h"
#include "Descriptions.h"
#include "Option.h"

namespace algos::config {

template <typename PT>
struct OptDistToNullInfinity : Option<bool, PT, &program_option_strings::kDistToNullIsInfinity,
        &algos::config::descriptions::kDDistToNullIsInfinity> {
    OptDistToNullInfinity() : Option<bool, PT, &program_option_strings::kDistToNullIsInfinity,
    &algos::config::descriptions::kDDistToNullIsInfinity>(false) {}
};

}
