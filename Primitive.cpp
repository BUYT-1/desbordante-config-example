#include "Primitive.h"

namespace algos {

void Primitive::Fit(StreamRef input_generator) {
    if (!GetNeededOptions().empty()) throw std::logic_error("Need to set all options first.");
    FitInternal(input_generator);
    SetExecuteOpts();
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

void Primitive::MakeOptionsAvailable(const std::vector<std::string> &option_names) {
    for (const auto& option_name : option_names) {
        auto it = possible_options_.find(option_name);
        assert(it != possible_options_.end());
        option_map_[option_name] = it->second;
    }
}

void Primitive::MakeOptionsAvailable(const std::string &parent_name, std::vector<std::string> const &option_names) {
    MakeOptionsAvailable(option_names);
    for (const auto& option_name : option_names) {
        opt_parents_[parent_name].emplace_back(option_name);
    }
}

void Primitive::ClearOptions() noexcept {
    option_map_.clear();
}

}  // namespace algos
