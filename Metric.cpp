#include <vector>
#include <iostream>

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
    opt_strings::kLhsIndices, config::descriptions::kDLhsIndices, TransformIndices
};

decltype(MetricVerifier::RhsIndices) MetricVerifier::RhsIndices{
    opt_strings::kRhsIndices, config::descriptions::kDRhsIndices, TransformIndices
};

decltype(MetricVerifier::Metric) MetricVerifier::Metric{
    opt_strings::kMetric, config::descriptions::kDMetric, [](auto value) {
        if (!(value == "euclidean" || value == "levenshtein" || value == "cosine"))
            throw std::invalid_argument("Unsupported metric");
    }
};

decltype(MetricVerifier::Algo) MetricVerifier::Algo{
    opt_strings::kMetricAlgorithm, config::descriptions::kDMetricAlgorithm, "brute", [](auto value) {
        if (!(value == "brute" || value == "approx" || value == "calipers"))
            throw std::invalid_argument("Unsupported metric");
    }
};

decltype(MetricVerifier::QGramLength) MetricVerifier::QGramLength{opt_strings::kQGramLength,
                                                                  config::descriptions::kDQGramLength, 2};

MetricVerifier::MetricVerifier() : Primitive() {
    AddPossibleOpts();
    MakeOptionsAvailable({opt_strings::kEqualNulls});
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

void MetricVerifier::SetExecuteOpts() {
    ClearOptions();
    MakeOptionsAvailable({opt_strings::kParameter, opt_strings::kDistToNullIsInfinity, opt_strings::kLhsIndices,
                          opt_strings::kRhsIndices});
}

void MetricVerifier::AddPossibleOpts() {
    auto check_ind = [this](auto value) { CheckIndices(value); };
    auto metric_post_set = [this](auto const& option, auto value) {
        std::string const &metricAlgorithm = opt_strings::kMetricAlgorithm;
        std::string const &qGramLength = opt_strings::kQGramLength;
        if (value == "levenshtein") {
            MakeOptionsAvailable(option.GetName(), {metricAlgorithm});
        } else if (value == "cosine") {
            MakeOptionsAvailable(option.GetName(), {metricAlgorithm, qGramLength});
        } else /*if (value == "euclidean") */ {
            if (rhs_indices_.size() != 1)
                MakeOptionsAvailable(option.GetName(), {metricAlgorithm});
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

    AddPossibleOption(config::EqualNulls.GetOption(&is_null_equal_null_));
    AddPossibleOption(config::NullDistInf.GetOption(&dist_to_null_infinity_));
    AddPossibleOption(config::Parameter.GetOption(&parameter_));
    AddPossibleOption(LhsIndices.GetOption(&lhs_indices_).SetSetter(check_ind));
    AddPossibleOption(RhsIndices.GetOption(&rhs_indices_).SetSetter(check_ind).SetPostSetFunc(
            [this](auto opt, auto) { MakeOptionsAvailable(opt.GetName(), {opt_strings::kMetric}); }));

    AddPossibleOption(Metric.GetOption(&metric_).SetPostSetFunc(metric_post_set));
    AddPossibleOption(Algo.GetOption(&algo_).SetSetter(algo_check));
    AddPossibleOption(QGramLength.GetOption(&q_));
}

}