#include <vector>
#include <iostream>

#include "Metric.h"
#include "Util.h"

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

void MetricVerifier::Fit(StreamRef input_generator) {
    if (!GetNeededOptions().empty()) throw std::logic_error("Need to set all options first.");
    // Some transformations
    processing_completed_ = true;
    SetExecOpts();
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

void MetricVerifier::SetExecOpts() {
    ClearOptions();
    MakeOptionsAvailable({opt_strings::kParameter, opt_strings::kDistToNullIsInfinity, opt_strings::kLhsIndices,
                          opt_strings::kRhsIndices});
}

void MetricVerifier::AddPossibleOpts() {
    AddPossibleOption(config::EqualNulls.GetOption([this](auto value) { is_null_equal_null_ = value; }));
    AddPossibleOption(config::NullDistInf.GetOption([this](auto value) { dist_to_null_infinity_ = value; }));
    AddPossibleOption(config::Parameter.GetOption([this](auto value) {parameter_ = value; }));
    AddPossibleOption(LhsIndices.GetOption([this](auto value) { CheckIndices(value); lhs_indices_ = value; }));
    AddPossibleOption(RhsIndices.GetOption([this](auto value) { CheckIndices(value); rhs_indices_ = value; },
                                           [this](auto) {
        MakeOptionsAvailable(opt_strings::kRhsIndices, {opt_strings::kMetric});
    }));

    auto metric_post_set = [this](std::string const &value) {
        std::string const& metric = opt_strings::kMetric;
        std::string const& metricAlgorithm = opt_strings::kMetricAlgorithm;
        std::string const& qGramLength = opt_strings::kQGramLength;
        if (value == "levenshtein") {
            MakeOptionsAvailable(metric, {metricAlgorithm});
        }
        else if (value == "cosine") {
            MakeOptionsAvailable(metric, {metricAlgorithm, qGramLength});
        }
        else /*if (value == "euclidean") */ {
            if (rhs_indices_.size() != 1)
                MakeOptionsAvailable(metric, {metricAlgorithm});
        }
    };
    AddPossibleOption(Metric.GetOption([this](auto value) { metric_ = value; }, metric_post_set));

    auto algo_set = [this](auto value) {
        auto const& rhs_val = rhs_indices_;
        auto const& metr_val = metric_;
        const auto algo_unusable = "Can't use this algorithm with this metric and RHS indices.";
        if (value == "approx") {
            if (metr_val == "euclidean" && rhs_val.size() == 1)
                throw std::invalid_argument(algo_unusable);
        }
        else if (value == "calipers") {
            if (!(metr_val == "euclidean" && rhs_val.size() == 2))
                throw std::invalid_argument(algo_unusable);
        }
        algo_ = value;
    };
    AddPossibleOption(Algo.GetOption(algo_set));

    AddPossibleOption(QGramLength.GetOption([this](auto value) { q_ = value; }));
}

}