#pragma once

#include <functional>
#include <utility>
#include "Primitive.h"

#include "ProgramOptionStrings.h"
#include "Option.h"
#include "Descriptions.h"
#include "CommonOptions.h"

namespace algos {

class MetricVerifier : public Primitive {
    // Note: it may make sense to split everything related to configuration
    // into some other class. However, this would make everything more
    // complicated, and there doesn't seem to be a practical use to justify
    // the complexity.
private:
    decltype(config::construct_eq_nulls())::element_type::Type is_null_equal_null_{};
    decltype(config::construct_parameter())::element_type::Type parameter_{};
    decltype(config::construct_null_dist_inf())::element_type::Type dist_to_null_infinity_{};
    std::vector<unsigned int> lhs_indices_{};
    std::vector<unsigned int> rhs_indices_{};
    std::string metric_{};
    std::string algo_{};
    unsigned int q_{};

    bool processing_completed_ = false;

    void CheckIndices(std::vector<unsigned int> const& value) const;
    void CleanIndices(std::vector<unsigned int>& value) const;
    void SetExecOpts();
    void AddPossibleOpts();

protected:
    void SetConfFields() override;

public:
    MetricVerifier();
    void Fit(StreamRef input_generator) override;
    unsigned long long Execute() override;
};

}  // namespace algos
