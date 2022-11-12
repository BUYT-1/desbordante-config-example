#include <vector>
#include <iostream>

#include "OptionCategories.h"
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

decltype(MetricVerifier::LhsIndices) MetricVerifier::LhsIndices{
        config::GetOptionInfoByName(opt_names::kLhsIndices), TransformIndices
};

decltype(MetricVerifier::RhsIndices) MetricVerifier::RhsIndices{
        config::GetOptionInfoByName(opt_names::kRhsIndices), TransformIndices
};

decltype(MetricVerifier::Metric) MetricVerifier::Metric{
        config::GetOptionInfoByName(opt_names::kMetric), [](auto value) {
        if (!(value == "euclidean" || value == "levenshtein" || value == "cosine"))
            throw std::invalid_argument("Unsupported metric");
    }
};

decltype(MetricVerifier::Algo) MetricVerifier::Algo{
        config::GetOptionInfoByName(opt_names::kMetricAlgorithm), "brute", [](auto value) {
        if (!(value == "brute" || value == "approx" || value == "calipers"))
            throw std::invalid_argument("Unsupported metric");
    }
};

decltype(MetricVerifier::QGramLength) MetricVerifier::QGramLength{config::GetOptionInfoByName(opt_names::kQGramLength), 2};

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
    auto true_func = [](...) { return true; };
    auto need_algo_and_q = [](auto value) { return value == "cosine"; };
    auto need_algo_only = [this](auto value) {
        return value == "levenshtein" || value == "euclidean" && rhs_indices_.size() == 1;
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
    RegisterOption(LhsIndices.GetOption(&lhs_indices_).SetInstanceCheck(check_ind));
    RegisterOption(
            RhsIndices.GetOption(&rhs_indices_).SetInstanceCheck(check_ind).SetConditionalOpts(GetOptAvailFunc(), {
                {true_func, config::GetOptionNames(Metric)}
    }));

    RegisterOption(Metric.GetOption(&metric_).SetConditionalOpts(GetOptAvailFunc(), {
        {need_algo_and_q, config::GetOptionNames(Algo, QGramLength)},
        {need_algo_only, config::GetOptionNames(Metric)}
    }));
    RegisterOption(Algo.GetOption(&algo_).SetInstanceCheck(algo_check));
    RegisterOption(QGramLength.GetOption(&q_));
}

}