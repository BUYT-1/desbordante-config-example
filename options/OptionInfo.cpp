#include <array>
#include <cassert>
#include <vector>

#include "ProgramOptionStrings.h"
#include "Descriptions.h"
#include "OptionInfo.h"

namespace algos::config {

std::array<std::pair<std::string_view, OptionInfo>, 8> const& GetOptionGroups() {
    static std::array<std::pair<std::string_view , OptionInfo>, 8> const option_groups = {
            std::make_pair(option_group_names::common, OptionInfo{opt_strings::kEqualNulls, descriptions::kDEqualNulls}),
            std::make_pair(option_group_names::common, OptionInfo{opt_strings::kDistToNullIsInfinity,
                                                                  descriptions::kDDistToNullIsInfinity}),
            std::make_pair(option_group_names::common, OptionInfo{opt_strings::kParameter, descriptions::kDParameter}),
            std::make_pair(option_group_names::mfd, OptionInfo{opt_strings::kLhsIndices,
                                                               config::descriptions::kDLhsIndices}),
            std::make_pair(option_group_names::mfd, OptionInfo{opt_strings::kRhsIndices,
                                                               config::descriptions::kDRhsIndices}),
            std::make_pair(option_group_names::mfd, OptionInfo{opt_strings::kMetric,
                                                               config::descriptions::kDMetric}),
            std::make_pair(option_group_names::mfd, OptionInfo{opt_strings::kMetricAlgorithm,
                                                               config::descriptions::kDMetricAlgorithm}),
            std::make_pair(option_group_names::mfd, OptionInfo{opt_strings::kQGramLength,
                                                               config::descriptions::kDQGramLength}),
    };
    return option_groups;
}

OptionInfo const& GetOptionInfo(std::string_view const group_name, std::string_view const option_name) {
    auto const& option_groups = GetOptionGroups();
    for (auto const& info : option_groups) {
        if (info.first == group_name && info.second.name_ == option_name) {
            return info.second;
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

OptionInfo::OptionInfo(std::string_view name, std::string_view description) : name_(name), description_(description) {}

}
