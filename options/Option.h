#pragma once

namespace algos::config {

template <typename T, typename PT, const std::string * const OptName, const std::string * const description>
struct Option {
public:
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

    /*void SetPy(PT& primitive, py::object value) {  // specialize for vector
        Set(primitive, py::extract<T>(value))
    }*/

    /*void AddToProgramOptions(po::options_description po) {
        // check that option with this name is not there, then
        po.add_options(*name, po::value<T>(), *description);
    }*/

    /*std::string GetJson() {
        ...
    }*/

    static constexpr auto name = OptName;
    T value_;
    bool is_set_ = false;
};

template <typename T, typename PT, const std::string * const name, const std::string * const description>
struct DefOption : public Option<T, PT, name, description> {
    explicit DefOption(T default_value) : default_value_(default_value) {}

    void SetDefault(PT& primitive) override {
        Option<T, PT, name, description>::Set(primitive, default_value_);
    }

private:
    T default_value_;
};

}
