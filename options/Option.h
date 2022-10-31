#pragma once

#include <optional>

namespace algos::config {

template <typename T, typename PT, const std::string * const OptName, const std::string * const Description>
struct Option {
public:
    using Type = T;

    Option() : description_(*Description) { }

    explicit Option(T default_value) : default_value_(default_value) { }

    //Option() : name_(*OptName), description_(*Description) { }

    //explicit Option(T default_value) : name_(*OptName), description_(*Description), default_value_(default_value) { }

    virtual void SetDefault(PT& primitive) {
        if (!default_value_.has_value())
            throw std::exception();
        Set(primitive, default_value_.value());
    }

    virtual void Set(PT& primitive, T value) {
        if (is_set_) Unset(primitive);
        value_ = value;
        is_set_ = true;
    }

    virtual void Unset(PT& primitive) {
        is_set_ = false;
    }

    void SetAny(PT& primitive, std::any value) {
        Set(primitive, std::any_cast<T>(value));
    }

    [[nodiscard]] std::string const& GetName() const {
        return *OptName;
    }

    [[nodiscard]] std::string const& GetDescription() const {
        return *Description;
    }

    [[nodiscard]] bool IsSet() const {
        return is_set_;
    }

    T const& GetValue() const {
        return value_;
    }

    /*virtual void SetPy(PT& primitive, py::object value) {
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
};

}
