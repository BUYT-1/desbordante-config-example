#pragma once
#include <string>
#include <unordered_map>

#include "OptionGroups.h"
#include "OptionNames.h"
#include "Descriptions.h"

namespace algos::config {

struct OptionInfo {
    OptionInfo(std::string_view category, std::string_view name, std::string_view description);

    [[nodiscard]] std::string GetCategory() const;
    [[nodiscard]] std::string GetName() const;
    [[nodiscard]] std::string GetDescription() const;

private:
    std::string_view category_; // for --help, displaying options in categories is easier to read
    std::string_view name_;
    std::string_view description_;
    friend OptionInfo const& GetOptionInfoByName(std::string_view option_name);
};

OptionInfo const& GetOptionInfoByName(std::string_view option_name);
std::array<OptionInfo const, 8> const& GetAllOptionInfo();

}  // namespace algos::config
