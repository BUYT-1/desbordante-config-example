#pragma once

#include <string>
#include <unordered_set>

namespace algos::config::util {

template <typename s> struct is_collection : std::false_type {};
template <template <class> class TP, typename ElT> struct is_collection<TP<ElT>> : std::true_type {
    using Type = ElT;
};

template <typename... Opts, typename Option>
void CollectUnsetOptions(std::unordered_set<std::string> const& available_options,
                     std::unordered_set<std::string>& uns_opts, Option& opt, Opts&... opts) {
    auto got = available_options.find(opt.GetName());
    if (got != available_options.end()) {
        if (!opt.IsSet()) uns_opts.insert(opt.GetName());
    }
    if constexpr (sizeof...(opts) != 0) CollectUnsetOptions(available_options, uns_opts, opts...);
}

template <typename... Opts>
std::unordered_set<std::string> GetUnsetOptions(std::unordered_set<std::string> const& available_options,
                                                Opts&... opts) {
    std::unordered_set<std::string> uns_opts;
    CollectUnsetOptions(available_options, uns_opts, opts...);
    return uns_opts;
}

// Tries to set an option's default value.
template <typename Option, typename... Opts>
bool SetOptDefault(std::string const& opt_name, Option& opt, Opts&... opts) {
    if (opt.GetName() == opt_name) {
        opt.SetDefault();
        return true;
    }
    if constexpr (sizeof...(opts) != 0) return SetOptDefault(opt_name, opts...);
    return false;
}

// Set an option from std::any by name
template <typename Option, typename... Opts>
bool SetOptAny(std::string const& opt_name, std::any const& value, Option& opt, Opts&... opts) {
    if (opt.GetName() == opt_name) {
        opt.SetAny(value);
        return true;
    }
    if constexpr (sizeof...(opts) != 0) return SetOptAny(opt_name, value, opts...);
    return false;
}

// Unsets option by name.
template<typename Option, typename... Opts>
bool UnsetOpt(std::string const& opt_name, Option& opt, Opts&... opts) {
    if (opt.GetName() == opt_name) {
        opt.Unset();
        return true;
    }
    if constexpr (sizeof...(opts) != 0) return UnsetOpt(opt_name, opts...);
    return false;
}

template <typename Option, typename FieldType, typename... Pairs>
void SetFieldsFromOpt(FieldType& field, Option& option, Pairs&... pairs) {
    if (option.IsSet()) {
        field = option.GetValue();
    }
    if constexpr (sizeof...(pairs) != 0) SetFieldsFromOpt(pairs...);
}

}
