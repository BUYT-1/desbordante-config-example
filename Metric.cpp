#include <cassert>
#include <vector>

#include "Metric.h"
#include "Util.h"

namespace posr = opt_strings;
namespace algos {

MetricVerifier::MetricVerifier() : Primitive(),
        opt_lhs_indices_(opt_strings::kLhsIndices, config::descriptions::kDLhsIndices,
                {.validate_ = [this](auto value) { CheckIndices(value); }, .transform_ = CleanIndices}),
        opt_rhs_indices_(opt_strings::kRhsIndices, config::descriptions::kDRhsIndices,
                {.validate_ = [this](auto value) { CheckIndices(value); }, .transform_ = CleanIndices,
                 .post_set_ = [this](auto value) {
                     AddAvailableOption(opt_strings::kRhsIndices, posr::kMetric);
                 }}),
        opt_metric_(opt_strings::kMetric, config::descriptions::kDMetric,
                       {.validate_ = [](std::string const &value) {
                           if (!(value == "euclidean" || value == "levenshtein" || value == "cosine"))
                               throw std::invalid_argument("Unsupported metric");
                       }, .post_set_ = [this](std::string const &value) {
                           assert(opt_rhs_indices_.IsSet());
                           if (value == "levenshtein") {
                               AddAvailableOption(opt_strings::kMetric, opt_strings::kMetricAlgorithm);
                           }
                           else if (value == "cosine") {
                               AddAvailableOption(opt_strings::kMetric, opt_strings::kMetricAlgorithm);
                               AddAvailableOption(opt_strings::kMetric, opt_strings::kQGramLength);
                           }
                           else /*if (value == "euclidean") */ {
                               if (opt_rhs_indices_.GetValue().size() != 1)
                                   AddAvailableOption(opt_strings::kMetric, opt_strings::kMetricAlgorithm);
                           }
                       }}),
        opt_algo_(opt_strings::kMetricAlgorithm, config::descriptions::kDMetricAlgorithm,
                     {.validate_ = [this](auto value) {
                         assert(opt_metric_.IsSet());
                         assert(opt_rhs_indices_.IsSet());
                         if (value == "approx") {
                             if (opt_metric_.GetValue() == "euclidean" && opt_rhs_indices_.GetValue().size() == 1)
                                 throw std::invalid_argument("Can't use this algorithm with this metric and "
                                                             "RHS indices.");
                         }
                         else if (value == "calipers") {
                             if (!(opt_metric_.GetValue() == "euclidean" && opt_rhs_indices_.GetValue().size() == 2))
                                 throw std::invalid_argument("Can't use this algorithm with this metric and "
                                                             "RHS indices.");
                         }
                         else {
                             if (value == "brute") return;
                             throw std::invalid_argument("Unknown algorithm.");
                         }}}),
        opt_q_(opt_strings::kQGramLength, config::descriptions::kDQGramLength, 2)
        {
    available_options_.insert(opt_strings::kEqualNulls);
}

void MetricVerifier::CleanIndices(std::vector<unsigned int>& value) {
    std::sort(value.begin(), value.end());
    value.erase(std::unique(value.begin(), value.end()), value.end());
}

void MetricVerifier::SetConfFields() {
    // A call to base::SetConfFields can be here
    config::util::SetFieldsFromOpt(
            is_null_equal_null_, opt_is_null_equal_null_,
            parameter_, opt_parameter_,
            dist_to_null_infinity_, opt_dist_to_null_infinity_,
            lhs_indices_, opt_lhs_indices_,
            rhs_indices_, opt_rhs_indices_,
            metric_, opt_metric_,
            algo_, opt_algo_,
            q_, opt_q_);
}

void MetricVerifier::Fit(StreamRef input_generator) {
    if (!GetNeededOptions().empty()) throw std::logic_error("Need to set all options first.");
    SetConfFields();
    // Some transformations
    processing_completed_ = true;
    available_options_ = std::unordered_set<std::string>{posr::kParameter, posr::kDistToNullIsInfinity,
                                                         posr::kLhsIndices, posr::kRhsIndices};
}

unsigned long long MetricVerifier::Execute() {
    if (!(processing_completed_ && GetNeededOptions().empty()))
        throw std::logic_error("Need to set all options first.");
    SetConfFields();
    // Processing
    return 12345;
}

bool MetricVerifier::SetOption(std::string const& option_name, std::any const& value) {
    if (available_options_.find(option_name) == available_options_.end())
        return false;
    if (config::util::SetOptAny(option_name, value, opt_is_null_equal_null_, opt_dist_to_null_infinity_,
                                opt_parameter_, opt_lhs_indices_, opt_metric_, opt_rhs_indices_, opt_algo_, opt_q_))
        return true; // if there is a base class, call its SetOption
    assert(0);
}

bool MetricVerifier::SetOption(std::string const& option_name) {
    if (available_options_.find(option_name) == available_options_.end())
        return false;
    else if (config::util::SetOptDefault(option_name, opt_is_null_equal_null_, opt_dist_to_null_infinity_,
                                         opt_parameter_, opt_lhs_indices_, opt_metric_, opt_rhs_indices_, opt_algo_,
                                         opt_q_))
        return true; // if there is a base class, call its SetOption
    assert(0);
}

[[nodiscard]] std::unordered_set<std::string> MetricVerifier::GetNeededOptions() const {
    // could call to the base class too (we don't know what other options it might have,
    // even though their names are in available_options_)
    return config::util::GetUnsetOptions(available_options_, opt_is_null_equal_null_, opt_dist_to_null_infinity_,
                                         opt_parameter_, opt_lhs_indices_, opt_metric_, opt_rhs_indices_, opt_algo_,
                                         opt_q_);
}

bool MetricVerifier::UnsetOption(std::string const& option_name) noexcept {
    if (available_options_.find(option_name) == available_options_.end())
        return false;
    else if (config::util::UnsetOpt(option_name, opt_is_null_equal_null_, opt_dist_to_null_infinity_,
                                    opt_parameter_, opt_lhs_indices_, opt_metric_, opt_rhs_indices_, opt_algo_,
                                    opt_q_)) {
        Primitive::ExcludeOptions(option_name);
        return true; // if there is a base class, call its UnsetOption
    }
    assert(0);
}

void MetricVerifier::CheckIndices(const std::vector<unsigned int>& value) const {
    if (value.empty()) {
        throw std::invalid_argument("Indices cannot be empty");
    }
    // OOB check
}

}