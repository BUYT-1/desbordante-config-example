#pragma once

#include <any>
#include <cassert>
#include <memory>
#include <filesystem>
#include <unordered_set>
#include <unordered_map>

#include "IDatasetStream.h"
#include "CSVParser.h"
#include "Option.h"

namespace algos {

class Primitive {
private:
    // For automatically making options unavailable (so they can't be set)
    // [see: Primitive::ExcludeOptions].
    std::unordered_map<std::string, std::vector<std::string>> opt_parents_{};
protected:
    using StreamRef = model::IDatasetStream &;
    std::unordered_map<std::string, std::shared_ptr<config::IOption>> option_map_{};
    std::unordered_map<std::string, std::shared_ptr<config::IOption>> possible_options_{};

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
    void SetOption(std::string const& option_name, std::any const& value);

    // Set the default value of an option.
    void SetOption(std::string const& option_name);

    // Set the value of an option from a Python object.
    /* virtual bool SetOption(string option_name, py::object obj) = 0 */

    // Get JSON description of the option.
    /* virtual std::string GetJson(string option_name) = 0 */

    // Return a po::options_description with all options for the primitive.
    /* virtual po::options_description GetOptionsDescription() = 0 */

    // Get options that need to be set
    [[nodiscard]] std::unordered_set<std::string> GetNeededOptions() const;

    void UnsetOption(std::string const& option_name) noexcept;

protected:
    virtual void SetConfFields() = 0;

    void MakeOptionsAvailable(std::string const& parent_name, std::vector<std::string> const& option_names);

    void MakeOptionsAvailable(std::vector<std::string> const& option_names);

    void AddPossibleOption(std::shared_ptr<config::IOption> option);

    // Remove options that were added by an option that was unset.
    void ExcludeOptions(std::string const& parent_option);

    template<typename T>
    T GetOptionValue(std::string const& opt_name) {
        auto it = option_map_.find(opt_name);
        assert(it != option_map_.end());
        auto& opt_ptr = it->second;
        assert(opt_ptr != nullptr);
        return opt_ptr->GetValue<T>();
    }
};

}