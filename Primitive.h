#pragma once

#include <any>
#include <cassert>
#include <memory>
#include <filesystem>
#include <unordered_set>
#include <unordered_map>

#include "CSVParser.h"
#include "IDatasetStream.h"
#include "OptionTypes.h"
#include "Option.h"

namespace algos {

class Primitive {
private:
    // For automatically making options unavailable (so they can't be set)
    // [see: Primitive::ExcludeOptions].
    std::unordered_map<std::string, std::vector<std::string>> opt_parents_{};
    std::unordered_map<std::string, std::shared_ptr<config::IOption>> option_map_{};
    std::unordered_map<std::string, std::shared_ptr<config::IOption>> possible_options_{};

    void MakeOptionsAvailable(std::string const& parent_name, std::vector<std::string> const& option_names);

    // Remove options that were added by an option that was unset.
    void ExcludeOptions(std::string const& parent_option);

protected:
    using StreamRef = model::IDatasetStream &;

public:
    Primitive() = default;

    Primitive(Primitive const &other) = delete;
    Primitive &operator=(Primitive const &other) = delete;
    Primitive(Primitive &&other) = delete;
    Primitive &operator=(Primitive &&other) = delete;

    virtual ~Primitive() = default;

    void Fit(StreamRef input_generator);

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
    std::function<void(std::string const&, std::vector<std::string> const&)> GetOptAvailFunc();

    virtual void FitInternal(StreamRef input_generator) = 0;

    virtual void MakeExecuteOptsAvailable() = 0;

    void MakeOptionsAvailable(std::vector<std::string> const& option_names);

    template<typename T>
    void RegisterOption(config::Option<T> option) {
        possible_options_[option.GetName()] = std::make_shared<config::Option<T>>(option);
    }

    void ClearOptions() noexcept;
};

}