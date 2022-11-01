#include <stdexcept>

#include "Option.h"
#include "ProgramOptionStrings.h"
#include "Descriptions.h"

namespace algos::config {

Option<bool> construct_null_dist_inf() {
    return {opt_strings::kDistToNullIsInfinity, descriptions::kDDistToNullIsInfinity, false};
}

Option<bool> construct_eq_nulls() {
    return {opt_strings::kEqualNulls, descriptions::kDEqualNulls, true};
}

Option<long double> construct_parameter() {
    return {opt_strings::kParameter, descriptions::kDParameter,
            {.validate_ = [](const long double &value) {
                if (!(value >= 0 && value <= 1))
                    throw std::invalid_argument("Out of range");
            }}, 0
    };
}

}