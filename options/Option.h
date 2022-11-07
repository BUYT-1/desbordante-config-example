#pragma once

#include <any>
#include <cassert>
#include <functional>
#include <optional>
#include <memory>
#include <vector>
#include <utility>

#include "IOption.h"

namespace algos::config {

template<typename T>
struct Option : public IOption {
public:
    Option(std::string name, std::string description, std::function<void(T &)> value_check,
           std::optional<T> default_value) : name_(std::move(name)), description_(std::move(description)),
           value_check_(std::move(value_check)), default_value_(std::move(default_value)) {}

    void SetDefault() override {
        if (!default_value_.has_value())
            throw std::exception();
        Set(default_value_.value());
    }

    void Set(T value) {
        if (is_set_) Unset();

        if (value_check_) value_check_(value);
        if (instance_check_) instance_check_(value);

        assert(value_ptr_ != nullptr);
        *value_ptr_ = value;
        is_set_ = true;
        if (post_set_) post_set_(*this, value);
    }

    void Unset() override {
        is_set_ = false;
    }

    void SetAny(std::any value) override {
        Set(std::any_cast<T>(value));
    }

    [[nodiscard]] std::string const& GetName() const override {
        return name_;
    }

    [[nodiscard]] std::string const& GetDescription() const {
        return description_;
    }

    [[nodiscard]] bool IsSet() const override {
        return is_set_;
    }

    Option & SetSetter(T* val_ptr, std::function<void(T &)> instance_check = {}) {
        instance_check_ = instance_check;
        value_ptr_ = val_ptr;
        assert(value_ptr_ != nullptr);
        return *this;
    }

    Option & SetPostSetFunc(std::function<void(Option const&, T const &)> post_set) {
        assert(post_set);
        post_set_ = post_set;
        return *this;
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
        po.add_options(name_, po::value<T>(), *Description);
    }*/

    /*virtual std::string GetJson() {
        ...
    }*/

private:
    bool is_set_ = false;
    std::string description_;
    std::string name_;
    std::optional<T> default_value_{};
    std::function<void(T &)> value_check_{};
    std::function<void(T &)> instance_check_{};
    T * value_ptr_;
    std::function<void(Option const&, T const &)> post_set_{};
};

}
