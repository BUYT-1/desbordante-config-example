#pragma once

namespace algos::config {

template <typename T, typename PT, const std::string * const OptName, const std::string * const Description>
struct Option {
public:
    using Type = T;

    virtual void SetDefault(PT& primitive) {
        throw std::exception();
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
        Set(primitive, py::extract<T>(value))
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
};

template <typename T, typename PT, const std::string * const OptName, const std::string * const Description>
struct VectorOption : Option<std::vector<T>, PT, OptName, Description> { // python lists require special treatment
    /*void SetPy(PT& primitive, py::list value) override {
        std::vector<T> elements;
        for (const auto& element : value) { // not exactly this loop, but just the idea
            elements.emplace_back(element);
        }
        Set(primitive, elements);
    }*/
};

template <typename T, typename PT, const std::string * const OptName, const std::string * const Description>
struct DefOption : public Option<T, PT, OptName, Description> {
    explicit DefOption(T default_value) : default_value_(default_value) {}

    void SetDefault(PT& primitive) override {
        Option<T, PT, OptName, Description>::Set(primitive, default_value_);
    }

    /*void AddToProgramOptions(po::options_description po) override {
        po.add_options(GetName(), po::value<T>()->default_value(default_value_), GetDescription());
    }*/

private:
    T default_value_;
};

}
