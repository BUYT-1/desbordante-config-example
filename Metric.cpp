#include <cassert>
#include <vector>

#include "Metric.h"
#include "Util.h"

namespace algos {

MetricVerifier::MetricVerifier() : Primitive() {
    available_options_.insert(program_option_strings::kEqualNulls);
}

void MetricVerifier::Fit(StreamRef input_generator) {
    namespace posr = program_option_strings;
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

}