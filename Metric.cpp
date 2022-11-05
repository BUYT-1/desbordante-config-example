#include <vector>
#include <iostream>

#include "Metric.h"
#include "Util.h"

namespace posr = opt_strings;
namespace algos {

MetricVerifier::MetricVerifier() : Primitive() {
    AddOption(opt_strings::kEqualNulls, config::construct_eq_nulls());
}

void MetricVerifier::CleanIndices(std::vector<unsigned int>& value) const {
    std::sort(value.begin(), value.end());
    value.erase(std::unique(value.begin(), value.end()), value.end());
}

void MetricVerifier::SetConfFields() {
    // A call to base::SetConfFields can be here
    config::util::SetFieldsFromOpt(option_map_,
            is_null_equal_null_, opt_strings::kEqualNulls,
            parameter_, opt_strings::kParameter,
            dist_to_null_infinity_, opt_strings::kDistToNullIsInfinity,
            lhs_indices_, opt_strings::kLhsIndices,
            rhs_indices_, opt_strings::kRhsIndices,
            metric_, opt_strings::kMetric,
            algo_, opt_strings::kMetricAlgorithm,
            q_, opt_strings::kQGramLength);
}

void MetricVerifier::Fit(StreamRef input_generator) {
    if (!GetNeededOptions().empty()) throw std::logic_error("Need to set all options first.");
    SetConfFields();
    // Some transformations
    processing_completed_ = true;
    AddExecOpts();
}

unsigned long long MetricVerifier::Execute() {
    if (!(processing_completed_ && GetNeededOptions().empty()))
        throw std::logic_error("Need to set all options first.");
    SetConfFields();
    // Processing
    return 12345;
}

void MetricVerifier::CheckIndices(const std::vector<unsigned int>& value) const {
    if (value.empty()) {
        throw std::invalid_argument("Indices cannot be empty");
    }
    // OOB check
}

void MetricVerifier::AddExecOpts() {
    AddOption(opt_strings::kParameter, config::construct_parameter());
    AddOption(opt_strings::kDistToNullIsInfinity, config::construct_null_dist_inf());

    auto ind_check = [this](auto value) { CheckIndices(value); };
    auto ind_clean = [this](auto value) { CleanIndices(value); };

    auto lhs = std::unique_ptr<config::IOption>(new config::Option<decltype(lhs_indices_)>(
            opt_strings::kLhsIndices, config::descriptions::kDLhsIndices, {},
            {.validate = ind_check, .transform = ind_clean}));
    AddOption(opt_strings::kLhsIndices, std::move(lhs));

    auto algo_validate = [this](auto value) {
        auto const& rhs_val = GetOptionValue<decltype(rhs_indices_)>(opt_strings::kRhsIndices);
        auto const& metr_val = GetOptionValue<decltype(metric_)>(opt_strings::kMetric);
        const auto algo_unusable = "Can't use this algorithm with this metric and RHS indices.";
        if (value == "approx") {
            if (metr_val == "euclidean" && rhs_val.size() == 1)
                throw std::invalid_argument(algo_unusable);
        }
        else if (value == "calipers") {
            if (!(metr_val == "euclidean" && rhs_val.size() == 2))
                throw std::invalid_argument(algo_unusable);
        }
        else {
            if (value == "brute") return;
            throw std::invalid_argument("Unknown algorithm.");
        }};

    auto metric_validate = [](std::string const &value) {
        if (!(value == "euclidean" || value == "levenshtein" || value == "cosine"))
            throw std::invalid_argument("Unsupported metric");
    };

    auto metric_post_set = [this, algo_validate](std::string const &value) {
        auto algo = std::unique_ptr<config::IOption>(new config::Option<decltype(algo_)>(
                opt_strings::kMetricAlgorithm, config::descriptions::kDMetricAlgorithm, {.validate = algo_validate}
        ));
        auto q = std::unique_ptr<config::IOption>(new config::Option<decltype(q_)>(
                opt_strings::kQGramLength, config::descriptions::kDQGramLength, 2
        ));
        std::string const& metric = opt_strings::kMetric;
        std::string const& metricAlgorithm = opt_strings::kMetricAlgorithm;
        std::string const& qGramLength = opt_strings::kQGramLength;
        auto const& rhs_val = GetOptionValue<decltype(rhs_indices_)>(opt_strings::kRhsIndices);
        if (value == "levenshtein") {
            AddOption(metric, metricAlgorithm, std::move(algo));
        }
        else if (value == "cosine") {
            AddOption(metric, metricAlgorithm, std::move(algo));
            AddOption(metric, qGramLength, std::move(q));
        }
        else /*if (value == "euclidean") */ {
            if (rhs_val.size() != 1)
                AddOption(metric, metricAlgorithm, std::move(algo));
        }
    };

    auto rhs_post_set = [this, metric_validate, metric_post_set](auto value) {
        auto metric = std::unique_ptr<config::IOption>(new config::Option<decltype(metric_)>(
                opt_strings::kMetric, config::descriptions::kDMetric, {.validate = metric_validate,
                        .post_set = metric_post_set}
        ));
        AddOption(opt_strings::kRhsIndices, posr::kMetric, std::move(metric));
    };
    auto rhs = std::unique_ptr<config::IOption>(new config::Option<decltype(rhs_indices_)>(
            opt_strings::kRhsIndices, config::descriptions::kDRhsIndices, {.validate = ind_check,
                    .transform = ind_clean,
                    .post_set = rhs_post_set}));
    AddOption(opt_strings::kRhsIndices, std::move(rhs));
}

}