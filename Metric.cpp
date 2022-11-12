#include <vector>
#include <iostream>

#include "OptionGroups.h"
#include "OptionInfo.h"
#include "Option.h"
#include "Metric.h"

namespace algos {

void TransformIndices(std::vector<unsigned int>& value) {
    if (value.empty()) {
        throw std::invalid_argument("Indices cannot be empty");
    }
    std::sort(value.begin(), value.end());
    value.erase(std::unique(value.begin(), value.end()), value.end());
}

static inline config::OptionInfo GetInfo(std::string_view const& name) {
    return config::GetOptionInfo(config::option_group_names::mfd, name);
}

decltype(MetricVerifier::LhsIndices) MetricVerifier::LhsIndices{
    GetInfo(opt_strings::kLhsIndices), TransformIndices
};

decltype(MetricVerifier::RhsIndices) MetricVerifier::RhsIndices{
    GetInfo(opt_strings::kRhsIndices), TransformIndices
};

decltype(MetricVerifier::Metric) MetricVerifier::Metric{
    GetInfo(opt_strings::kMetric), [](auto value) {
        if (!(value == "euclidean" || value == "levenshtein" || value == "cosine"))
            throw std::invalid_argument("Unsupported metric");
    }
};

decltype(MetricVerifier::Algo) MetricVerifier::Algo{
    GetInfo(opt_strings::kMetricAlgorithm), "brute", [](auto value) {
        if (!(value == "brute" || value == "approx" || value == "calipers"))
            throw std::invalid_argument("Unsupported metric");
    }
};

decltype(MetricVerifier::QGramLength) MetricVerifier::QGramLength{GetInfo(opt_strings::kQGramLength), 2};

MetricVerifier::MetricVerifier() : Primitive() {
    RegisterOptions();
    MakeOptionsAvailable(config::GetOptionNames(config::EqualNulls));
}

void MetricVerifier::FitInternal(StreamRef input_generator) {
    // Some transformations
    processing_completed_ = true;
}

unsigned long long MetricVerifier::Execute() {
    if (!(processing_completed_ && GetNeededOptions().empty()))
        throw std::logic_error("Need to set all options first.");
    // Processing
    return 12345;
}

void MetricVerifier::CheckIndices(const std::vector<unsigned int>& value) const {
    // OOB check
}

void MetricVerifier::MakeExecuteOptsAvailable() {
    ClearOptions();
    MakeOptionsAvailable(config::GetOptionNames(config::Parameter, config::NullDistInf, LhsIndices, RhsIndices));
}

void MetricVerifier::RegisterOptions() {
    auto check_ind = [this](auto value) { CheckIndices(value); };
    auto metric_post_set = [this](auto const& option, auto value) {
        if (value == "levenshtein") {
            MakeOptionsAvailable(option.GetName(), config::GetOptionNames(Algo));
        } else if (value == "cosine") {
            MakeOptionsAvailable(option.GetName(), config::GetOptionNames(Algo, QGramLength));
        } else /*if (value == "euclidean") */ {
            if (rhs_indices_.size() != 1)
                MakeOptionsAvailable(option.GetName(), config::GetOptionNames(Algo));
        }
    };

    auto algo_check = [this](auto value) {
        const auto algo_unusable = "Can't use this algorithm with this metric and RHS indices.";
        if (value == "approx") {
            if (metric_ == "euclidean" && rhs_indices_.size() == 1)
                throw std::invalid_argument(algo_unusable);
        }
        else if (value == "calipers") {
            if (!(metric_ == "euclidean" && rhs_indices_.size() == 2))
                throw std::invalid_argument(algo_unusable);
        }
    };

    RegisterOption(config::EqualNulls.GetOption(&is_null_equal_null_));
    RegisterOption(config::NullDistInf.GetOption(&dist_to_null_infinity_));
    RegisterOption(config::Parameter.GetOption(&parameter_));
    RegisterOption(LhsIndices.GetOption(&lhs_indices_).SetSetter(check_ind));
    RegisterOption(RhsIndices.GetOption(&rhs_indices_).SetSetter(check_ind).SetPostSetFunc(
            [this](auto opt, auto) { MakeOptionsAvailable(opt.GetName(), config::GetOptionNames(Metric)); }));

    RegisterOption(Metric.GetOption(&metric_).SetPostSetFunc(metric_post_set));
    RegisterOption(Algo.GetOption(&algo_).SetSetter(algo_check));
    RegisterOption(QGramLength.GetOption(&q_));
}

}