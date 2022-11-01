#pragma once

#include <any>
#include <functional>
#include <optional>
#include <utility>

#include "Util.h"

namespace algos::config {

template <typename T>
struct OptionFuncs {
    std::function<void(T const &)> validate_{};
    std::function<void(T &)> transform_{};
    std::function<void(T const &)> post_set_{};
};

template<typename T>
struct Option {
public:
    using Type = T;

    Option(std::string name, std::string description, OptionFuncs<T> funcs, std::optional<T> default_value) :
    name_(std::move(name)), description_(std::move(description)), validate_(funcs.validate_),
    transform_(funcs.transform_), post_set_(funcs.post_set_), default_value_(std::move(default_value)) {}

    Option(std::string name, std::string description, OptionFuncs<T> funcs) : Option(name, description, funcs, {}) {}

    Option(std::string name, std::string description, std::optional<T> default_value) : Option(name, description, {},
                                                                                               default_value) {}

    virtual void SetDefault() {
        if (!default_value_.has_value())
            throw std::exception();
        Set(default_value_.value());
    }

    virtual void Set(T value) {
        if (is_set_) Unset();

        if (validate_) { validate_(value); }
        if (transform_) transform_(value);
        value_ = value;
        is_set_ = true;
        if (post_set_) post_set_(value);
    }

    virtual void Unset() {
        is_set_ = false;
    }

    void SetAny(std::any value) {
        Set(std::any_cast<T>(value));
    }

    [[nodiscard]] std::string const& GetName() const {
        return name_;
    }

    [[nodiscard]] std::string const& GetDescription() const {
        return description_;
    }

    [[nodiscard]] bool IsSet() const {
        return is_set_;
    }

    T const& GetValue() const {
        return value_;
    }

    /*virtual void SetPy(py::object value) {
        if constexpr (is_collection<T>{}) {
            using ElType = is_collection<T>::Type;
            T elements{};
            for (auto const& el : value) {
                elements.emplace_back(py::extract<ElType>(value));
            }
            Set(primitive, elements);
        }
        else {
            Set(primitive, py::extract<T>(value));
        }
    }*/

    /*virtual void AddToProgramOptions(po::options_description po) {
        po.add_options(*OptName, po::value<T>(), *Description);
    }*/

    /*virtual std::string GetJson() {
        ...
    }*/

private:
    T value_;
    bool is_set_ = false;
    std::string description_;
    std::string name_;
    std::optional<T> default_value_{};
    std::function<void(T const &)> validate_{};
    std::function<void(T &)> transform_{};
    std::function<void(T const &)> post_set_{};
};

}
