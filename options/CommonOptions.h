#pragma once
#include "Option.h"

namespace algos::config {
std::shared_ptr<Option<bool>> construct_eq_nulls();
std::shared_ptr<Option<bool>> construct_null_dist_inf();
std::shared_ptr<Option<long double>> construct_parameter();
}