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

    template <typename T>
    T GetValue() const {
        return std::any_cast<T>(GetValueAny());
    }

    virtual ~IOption() = default;

protected:
    [[nodiscard]] virtual std::any GetValueAny() const = 0;
};

template <typename T>
struct OptionFuncs {
    std::function<void(T const &)> validate{};
    std::function<void(T &)> transform{};
    std::function<void(T const &)> post_set{};
};

template<typename T>
struct Option : public IOption {
public:
    using Type = T;

    Option(std::string name, std::string description, std::optional<T> default_value = {},
           OptionFuncs<T> funcs = {}) : name_(std::move(name)), description_(std::move(description)),
           validate_(std::move(funcs.validate)), transform_(std::move(funcs.transform)),
           post_set_(std::move(funcs.post_set)), default_value_(std::move(default_value)) {}

    Option(std::string name, std::string description, OptionFuncs<T> funcs = {}) : Option(name, description, {}, funcs)
    {}

    void SetDefault() override {
        if (!default_value_.has_value())
            throw std::exception();
        Set(default_value_.value());
    }

    void Set(T value) {
        if (is_set_) Unset();

        if (validate_) { validate_(value); }
        if (transform_) transform_(value);
        value_ = value;
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

    [[nodiscard]] std::any GetValueAny() const override {
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
