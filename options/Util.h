#pragma once

#include <string>
#include <unordered_set>

namespace algos::config::util {

// Collects options that need to be set.
void GetUnsetOptions(std::unordered_set<std::string> const& available_options,
                     std::unordered_set<std::string>& uns_opts) { }

template <typename... Opts, typename Option>
void GetUnsetOptions(std::unordered_set<std::string> const& available_options,
                     std::unordered_set<std::string>& uns_opts, Option& opt, Opts&... opts) {
    auto got = available_options.find(opt.GetName());
    if (got != available_options.end()) {
        if (!opt.IsSet()) uns_opts.insert(opt.GetName());
    }
    GetUnsetOptions(available_options, uns_opts, opts...);
}

template <typename... Opts>
std::unordered_set<std::string> GetUnsetOptions(std::unordered_set<std::string> const& available_options,
                                                Opts&... opts) {
    std::unordered_set<std::string> uns_opts;
    GetUnsetOptions(available_options, uns_opts, opts...);
    return uns_opts;
}

// Tries to set an option's default value.
template <typename PT>
bool SetOptDefault(PT& primitive, std::string const& opt_name) {
    return false;
}

template <typename PT, typename Option, typename... Opts>
bool SetOptDefault(PT& primitive, std::string const& opt_name, Option& opt, Opts&... opts) {
    if (opt.GetName() == opt_name) {
        opt.SetDefault(primitive);
        return true;
    }
    return SetOptDefault(primitive, opt_name, opts...);
}

// Set an option from std::any
template <typename PT>
bool SetOptAny(PT& primitive, std::string const& opt_name, std::any const& value) {
    return false;
}

template <typename PT, typename Option, typename... Opts>
bool SetOptAny(PT& primitive, std::string const& opt_name, std::any const& value, Option& opt, Opts&... opts) {
    if (opt.GetName() == opt_name) {
        opt.SetAny(primitive, value);
        return true;
    }
    return SetOptAny(primitive, opt_name, value, opts...);
}

// Unsets option by name.
template <typename PT>
bool UnsetOpt(PT& primitive, std::string const& opt_name) {
    return false;
}

template<typename PT, typename Option, typename... Opts>
bool UnsetOpt(PT& primitive, std::string const& opt_name, Option& opt, Opts&... opts) {
    if (opt.GetName() == opt_name) {
        opt.Unset(primitive);
        return true;
    }
    return UnsetOpt(primitive, opt_name, opts...);
}

// Convenience function to set a primitive's fields using values in its option struct.
void SetFieldsFromOpt() { }

template <typename Option, typename FieldType, typename... Pairs>
void SetFieldsFromOpt(FieldType& field, Option& option, Pairs&... pairs) {
    if (option.IsSet()) {
        field = option.GetValue();
    }
    SetFieldsFromOpt(pairs...);
}

}
