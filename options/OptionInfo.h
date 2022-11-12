#pragma once
#include <string>
#include <unordered_map>

#include "OptionGroups.h"
#include "ProgramOptionStrings.h"
#include "Descriptions.h"

namespace algos::config {

struct OptionInfo {
    OptionInfo(std::string_view name, std::string_view description) : name_(name), description_(description) {}

    std::string_view name_;
    std::string_view description_;
};

OptionInfo const& GetOptionInfo(std::string_view const group_name, std::string_view const option_name);
std::array<std::pair<std::string_view, OptionInfo>, 8> const& GetOptionGroups();

}  // namespace algos::config
