#include "Primitive.h"

namespace algos {

void Primitive::AddAvailableOption(const std::string &parent_name, const std::string &option_name) {
    opt_parents[parent_name].emplace_back(option_name);
    available_options_.insert(option_name);
}

void Primitive::AddAvailableOption(const std::string &option_name) {
    available_options_.insert(option_name);
}

void Primitive::ExcludeOptions(std::string const& parent_option) {
    auto it = opt_parents.find(parent_option);
    if (it == opt_parents.end()) return;

    for (auto const& option : it->second) {
        UnsetOption(option);
        available_options_.erase(option);
    }
    opt_parents.erase(parent_option);
}

}  // namespace algos
