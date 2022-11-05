#include <stdexcept>
#include <memory>

#include "Option.h"
#include "ProgramOptionStrings.h"
#include "Descriptions.h"

namespace algos::config {

std::shared_ptr<Option<bool>> construct_eq_nulls() {
    return std::make_shared<Option<bool>>(opt_strings::kEqualNulls, descriptions::kDEqualNulls, true);
}

std::shared_ptr<Option<bool>> construct_null_dist_inf() {
    return std::make_shared<Option<bool>>(opt_strings::kDistToNullIsInfinity, descriptions::kDDistToNullIsInfinity,
            false);
}

std::shared_ptr<Option<long double>> construct_parameter() {
    return std::shared_ptr<Option<long double>>(new Option<long double>(
            opt_strings::kParameter, descriptions::kDParameter, 0,
            {.validate = [](const long double &value) {
                if (!(value >= 0 && value <= 1))
                    throw std::invalid_argument("Out of range");
            }})
    );
}

}