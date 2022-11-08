#pragma once

#include <any>
#include <cassert>
#include <functional>
#include <optional>
#include <memory>
#include <vector>
#include <utility>

#include "OptionInfo.h"
#include "IOption.h"

namespace algos::config {

template<typename T>
struct Option : public IOption {
public:
    Option(OptionInfo info, T* value_ptr, std::function<void(T &)> value_check,
           std::optional<T> default_value) : info_(info), value_ptr_(value_ptr), value_check_(std::move(value_check)),
           default_value_(std::move(default_value)) {}

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
        if (opt_add_func_) {
            for (auto const& [cond, opts] : opt_cond_) {
                assert(cond);
                if (cond(value)) {
                    opt_add_func_(info_.GetName(), opts);
                    break;
                }
            }
        }
    }

    void Unset() override {
        is_set_ = false;
    }

    void SetAny(std::any value) override {
        Set(std::any_cast<T>(value));
    }

    [[nodiscard]] std::string GetName() const override {
        return info_.GetName();
    }

    [[nodiscard]] std::string GetDescription() const {
        return info_.GetDescription();
    }

    [[nodiscard]] bool IsSet() const override {
        return is_set_;
    }

    Option & SetSetter(std::function<void(T &)> instance_check = {}) {
        instance_check_ = instance_check;
        return *this;
    }

    Option & SetConditionalOpts(
            std::function<void(std::string const&, std::vector<std::string> const&)> const& add_opts,
            std::vector<std::pair<std::function<bool(T const& val)>, std::vector<std::string>>> opt_cond) {
        assert(add_opts);
        assert(!opt_cond.empty());
        opt_cond_ = opt_cond;
        opt_add_func_ = add_opts;
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

    /*std::string GetJson() {
        return info_.json_;
    }*/

private:
    bool is_set_ = false;
    OptionInfo info_;
    std::optional<T> default_value_{};
    std::function<void(T &)> value_check_{};
    std::function<void(T &)> instance_check_{};
    T * value_ptr_;
    std::vector<std::pair<std::function<bool(T const& val)>, std::vector<std::string>>> opt_cond_{};
    std::function<void(std::string const&, std::vector<std::string> const&)> opt_add_func_{};
};

}
