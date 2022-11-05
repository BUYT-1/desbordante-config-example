#pragma once

#include <string>
#include <unordered_set>

#include "Option.h"

namespace algos::config::util {

template <typename s> struct is_collection : std::false_type {};
template <template <class> class TP, typename ElT> struct is_collection<TP<ElT>> : std::true_type {
    using Type = ElT;
};

template <typename FieldType, typename... Pairs>
void SetFieldsFromOpt(std::unordered_map<std::string, std::shared_ptr<config::IOption>> const& option_map,
                      FieldType& field, std::string const& option_name, Pairs&... pairs) {
    auto it = option_map.find(option_name);
    if (it != option_map.end())
    {
        auto const &option = *it->second;
        if (option.IsSet()) {
            field = option.template GetValue<FieldType>();
        }
    }
    if constexpr (sizeof...(pairs) != 0) SetFieldsFromOpt(option_map, pairs...);
}

}
