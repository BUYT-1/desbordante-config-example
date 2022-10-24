#include <cassert>
#include <vector>

#include "Metric.h"
#include "Util.h"

namespace {
namespace posr = program_option_strings;
}

namespace algos {

MetricVerifier::MetricVerifier() : Primitive() {
    available_options_.insert(program_option_strings::kEqualNulls);
}

void MetricVerifier::Fit(StreamRef input_generator) {
    if (!is_null_equal_null_.is_set_) throw std::exception();
    // Some transformations
    processing_completed_ = true;
    available_options_ = std::unordered_set<std::string>{posr::kParameter, posr::kDistToNullIsInfinity,
                                                         posr::kLhsIndices, posr::kRhsIndices};
}

unsigned long long MetricVerifier::Execute() {
    if (!(processing_completed_ && GetNeededOptions().empty())) throw std::exception();
    return 12345;
}

bool MetricVerifier::SetOption(std::string const& option_name, std::any const& value) {
    if (available_options_.find(option_name) == available_options_.end())
        return false; // this could also be a call to the base class's UnsetOption
    if (config::util::SetOptAny(*this, option_name, value, is_null_equal_null_, dist_to_null_infinity_, parameter_,
                                lhs_indices_, metric_, rhs_indices_, algo_, q_)) return true;
    assert(0);
}

bool MetricVerifier::SetOption(std::string const& option_name) {
    if (available_options_.find(option_name) == available_options_.end())
        return false; // this could also be a call to the base class's UnsetOption
    else if (config::util::SetOptDefault(*this, option_name, is_null_equal_null_, dist_to_null_infinity_, parameter_,
                                    lhs_indices_, metric_, rhs_indices_, algo_, q_))
        return true;
    assert(0);
}

[[nodiscard]] std::unordered_set<std::string> MetricVerifier::GetNeededOptions() const {
    return config::util::GetUnsetOptions(available_options_, is_null_equal_null_, dist_to_null_infinity_, parameter_,
                                         lhs_indices_, metric_, rhs_indices_, algo_, q_);
}

bool MetricVerifier::UnsetOption(std::string const& option_name) noexcept {
    if (available_options_.find(option_name) == available_options_.end())
        return false; // this could also be a call to the base class's UnsetOption
    else if (config::util::UnsetOpt(*this, option_name, is_null_equal_null_, dist_to_null_infinity_, parameter_,
                                    lhs_indices_, metric_, rhs_indices_, algo_, q_)) {
        Primitive::ExcludeOptions(option_name);
        return true;
    }
    assert(0);
}

void MetricVerifier::OptLhsIndices::Set(MetricVerifier& primitive, std::vector<unsigned int> value) {
    if (value.empty()) throw std::exception();
    // OOB check should be here
    std::sort(value.begin(), value.end());
    value.erase(std::unique(value.begin(), value.end()), value.end());
    OptLhsType::Set(primitive, std::move(value));
}

void MetricVerifier::OptMetric::Set(MetricVerifier& primitive, std::string value) {
    if (!(value == "euclidean" || value == "cosine" || value == "levenshtein")) throw std::exception();
    OptMetricType::Set(primitive, value);
    if (value == "cosine") primitive.AddAvailableOption(*name, posr::kQGramLength);
    if (!(value == "euclidean" && primitive.rhs_indices_.value_.size() == 1))
        primitive.AddAvailableOption(*name, posr::kMetricAlgorithm);
}

void MetricVerifier::OptRhsIndices::Set(MetricVerifier& primitive, std::vector<unsigned int> value) {
    if (value.empty()) throw std::exception();
    if ((primitive.metric_.value_ == "levenshtein" || primitive.metric_.value_ == "cosine")
        && value.size() != 1) {
        throw std::exception();
    }
    // OOB check should be here
    std::sort(value.begin(), value.end());
    value.erase(std::unique(value.begin(), value.end()), value.end());
    OptRhsType::Set(primitive, value);
    if (!(primitive.metric_.value_ == "euclidean" && value.size() == 1))
        primitive.AddAvailableOption(*name, posr::kMetric);
}

void MetricVerifier::OptMetricAlgo::Set(MetricVerifier & primitive, std::string value) {
    if (value == "brute") {
        OptMetricAlgoType::Set(primitive, std::move(value));
    }
    else if (value == "approx") {
        if (primitive.metric_.value_ == "euclidean" && primitive.rhs_indices_.value_.size() == 1)
            throw std::exception();
        OptMetricAlgoType::Set(primitive, std::move(value));
    }
    else if (value == "calipers") {
        if (!(primitive.metric_.value_ == "euclidean" && primitive.rhs_indices_.value_.size() == 2))
            throw std::exception();
        OptMetricAlgoType::Set(primitive, std::move(value));
    }
    else {
        throw std::exception();
    }
}

void MetricVerifier::OptQ::Set(MetricVerifier& primitive, unsigned int value) {
    if (!(primitive.metric_.is_set_ && primitive.metric_.value_ == "cosine"))
        throw std::exception();
    OptQType::Set(primitive, value);
}

}