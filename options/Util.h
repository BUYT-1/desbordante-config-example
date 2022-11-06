#pragma once

#include <string>
#include <unordered_set>

#include "Option.h"

namespace algos::config::util {

template <typename s> struct is_collection : std::false_type {};
template <template <class> class TP, typename ElT> struct is_collection<TP<ElT>> : std::true_type {
    using Type = ElT;
};

}
