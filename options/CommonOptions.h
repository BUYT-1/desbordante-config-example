#pragma once
#include "Option.h"

namespace algos::config {
std::unique_ptr<Option<bool>> construct_eq_nulls();
std::unique_ptr<Option<bool>> construct_null_dist_inf();
std::unique_ptr<Option<long double>> construct_parameter();
}