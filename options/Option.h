#pragma once

#include <any>
#include <functional>
#include <optional>
#include <utility>

namespace algos::config {

struct IOption {
    virtual void SetDefault() = 0;
    virtual void SetAny(std::any value) = 0;
    virtual void Unset() = 0;
    [[nodiscard]] virtual bool IsSet() const = 0;
    [[nodiscard]] virtual std::string const& GetName() const = 0;
    virtual ~IOption() = default;
};

template<typename T>
struct Option : public IOption {
public:
    using Type = T;

    Option(std::string name, std::string description, std::function<void(T &)> set, std::optional<T> default_value = {},
           std::function<void(T const &)> post_set = {}) : name_(std::move(name)), description_(std::move(description)),
           default_value_(std::move(default_value)), set_(set), post_set_(post_set) {}

    void SetDefault() override {
        if (!default_value_.has_value())
            throw std::exception();
        Set(default_value_.value());
    }

    void Set(T value) {
        if (is_set_) Unset();

        set_(value);
        is_set_ = true;
        if (post_set_) post_set_(value);
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
    std::function<void(T &)> set_{};
    std::function<void(T const &)> post_set_{};
};

}
