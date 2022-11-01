#pragma once
#include "Option.h"

namespace algos::config {
Option<bool> construct_eq_nulls();
Option<bool> construct_null_dist_inf();
Option<long double> construct_parameter();
}