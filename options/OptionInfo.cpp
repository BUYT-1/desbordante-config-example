#include <array>
#include <cassert>
#include <vector>

#include "OptionNames.h"
#include "Descriptions.h"
#include "OptionInfo.h"

namespace algos::config {

std::array<OptionInfo const, 8> const& GetAllOptionInfo() {
    static std::array<OptionInfo const, 8> const all_option_info = {
            OptionInfo{option_category_names::common, opt_names::kEqualNulls, descriptions::kDEqualNulls},
            OptionInfo{option_category_names::common, opt_names::kDistToNullIsInfinity,
                       descriptions::kDDistToNullIsInfinity},
            OptionInfo{option_category_names::common, opt_names::kParameter, descriptions::kDParameter},
            OptionInfo{option_category_names::mfd, opt_names::kLhsIndices, descriptions::kDLhsIndices},
            OptionInfo{option_category_names::mfd, opt_names::kRhsIndices, descriptions::kDRhsIndices},
            OptionInfo{option_category_names::mfd, opt_names::kMetric, descriptions::kDMetric},
            OptionInfo{option_category_names::mfd, opt_names::kMetricAlgorithm, descriptions::kDMetricAlgorithm},
            OptionInfo{option_category_names::mfd, opt_names::kQGramLength, descriptions::kDQGramLength},
    };
    return all_option_info;
}

OptionInfo const& GetOptionInfoByName(std::string_view const option_name) {
    auto const& option_groups = GetAllOptionInfo();
    for (auto const& info : option_groups) {
        if (info.name_ == option_name) {
            return info;
        }
    }
    assert(0);
}

std::string OptionInfo::GetName() const {
    return std::string(name_);
}

std::string OptionInfo::GetDescription() const {
    return std::string(description_);
}

std::string OptionInfo::GetCategory() const {
    return std::string(category_);
}

OptionInfo::OptionInfo(std::string_view category, std::string_view name, std::string_view description) : name_(name),
description_(description), category_(category) {}

}
