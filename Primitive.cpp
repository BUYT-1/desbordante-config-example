#include "Primitive.h"

namespace algos {
void Primitive::AddOption(const std::string &option_name, std::unique_ptr<config::IOption> option) {
    option_map_.insert(std::make_pair(option_name, std::move(option)));
}

void Primitive::AddOption(const std::string &parent_name, const std::string &option_name,
                          std::unique_ptr<config::IOption> option) {
    option_map_.insert(std::make_pair(option_name, std::move(option)));
    opt_parents_[parent_name].emplace_back(option_name);
}

void Primitive::ExcludeOptions(std::string const& parent_option) {
    auto it = opt_parents_.find(parent_option);
    if (it == opt_parents_.end()) return;

    for (auto const& option_name : it->second) {
        option_map_.erase(option_name);
        UnsetOption(option_name);
    }
    opt_parents_.erase(parent_option);
}

std::unordered_set<std::string> Primitive::GetNeededOptions() const {
    std::unordered_set<std::string> needed{};
    for (auto const& [name, opt] : option_map_) {
        if (!opt->IsSet())
            needed.insert(name);
    }
    return needed;
}

void Primitive::SetOption(const std::string &option_name) {
    auto it = option_map_.find(option_name);
    if (it == option_map_.end())
        throw std::invalid_argument("Invalid option");
    it->second->SetDefault();
}

void Primitive::SetOption(const std::string &option_name, const std::any &value) {
    auto it = option_map_.find(option_name);
    if (it == option_map_.end())
        throw std::invalid_argument("Invalid option");
    it->second->SetAny(value);
}

void Primitive::UnsetOption(const std::string &option_name) noexcept {
    auto it = option_map_.find(option_name);
    if (it == option_map_.end())
        return;
    it->second->Unset();
    ExcludeOptions(option_name);
}

}  // namespace algos
