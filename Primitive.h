#pragma once

#include <any>
#include <memory>
#include <filesystem>
#include <unordered_set>
#include <unordered_map>

#include "IDatasetStream.h"
#include "CSVParser.h"

namespace algos {

class Primitive {
private:
    // For automatically making options unavailable (so they can't be set)
    // [see: Primitive::ExcludeOptions].
    std::unordered_map<std::string, std::vector<std::string>> opt_parents{};
protected:
    using StreamRef = model::IDatasetStream &;
    // The set of all options which can be set from the outside when the primitive is in some state.
    std::unordered_set<std::string> available_options_{};

public:
    Primitive() = default;

    Primitive(Primitive const &other) = delete;
    Primitive &operator=(Primitive const &other) = delete;
    Primitive(Primitive &&other) = delete;
    Primitive &operator=(Primitive &&other) = delete;

    virtual ~Primitive() = default;

    // Transform raw data into a format suitable for a particular primitive.
    virtual void Fit(StreamRef input_generator) = 0;

    virtual unsigned long long Execute() = 0;

    // Set the value of an option.
    virtual bool SetOption(std::string const& option_name, std::any const& value) = 0;

    // Set the default value of an option.
    virtual bool SetOption(std::string const& option_name) = 0;

    // Set the value of an option from a Python object.
    /* virtual bool SetOption(string option_name, py::object obj) = 0 */

    // Get JSON description of the option.
    /* virtual std::string GetJson(string option_name) = 0 */

    // Return a po::options_description with all options for the primitive.
    /* virtual po::options_description GetOptionsDescription() = 0 */

    // Get options that need to be set
    [[nodiscard]] virtual std::unordered_set<std::string> GetNeededOptions() const = 0;

    virtual bool UnsetOption(std::string const& option_name) noexcept = 0;

protected:
    virtual void SetConfFields() = 0;

    // Called by some options to add more options.
    void AddAvailableOption(std::string const& parent_name, std::string const& option_name) {
        opt_parents[parent_name].emplace_back(option_name);
        available_options_.insert(option_name);
    }

    // Remove options that were added by an option that was unset.
    void ExcludeOptions(std::string const& parent_option) {
        auto it = opt_parents.find(parent_option);
        if (it == opt_parents.end()) return;

        for (auto const& option : it->second) {
            UnsetOption(option);
            available_options_.erase(option);
        }
        opt_parents.erase(parent_option);
    }
};

}