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
    decltype(config::construct_eq_nulls())::Type is_null_equal_null_{};
    decltype(config::construct_parameter())::Type parameter_{};
    decltype(config::construct_null_dist_inf())::Type dist_to_null_infinity_{};
    std::vector<unsigned int> lhs_indices_{};
    std::vector<unsigned int> rhs_indices_{};
    std::string metric_{};
    std::string algo_{};
    unsigned int q_{};

    bool processing_completed_ = false;

    void CheckIndices(std::vector<unsigned int> const& value) const;
    static void CleanIndices(std::vector<unsigned int>&);

protected:
    config::Option<decltype(is_null_equal_null_)> opt_is_null_equal_null_{config::construct_eq_nulls()};
    config::Option<decltype(parameter_)> opt_parameter_{config::construct_parameter()};
    config::Option<decltype(dist_to_null_infinity_)> opt_dist_to_null_infinity_{config::construct_null_dist_inf()};
    config::Option<decltype(lhs_indices_)> opt_lhs_indices_;
    config::Option<decltype(rhs_indices_)> opt_rhs_indices_;
    config::Option<decltype(metric_)> opt_metric_;
    config::Option<decltype(algo_)> opt_algo_;
    config::Option<decltype(q_)> opt_q_;

    void SetConfFields() override;

public:
    MetricVerifier();
    void Fit(StreamRef input_generator) override;
    unsigned long long Execute() override;
    bool SetOption(std::string const& option_name, std::any const& value) override;
    bool SetOption(std::string const& option_name) override;
    [[nodiscard]] std::unordered_set<std::string> GetNeededOptions() const override;
    bool UnsetOption(std::string const& option_name) noexcept override;
};

}  // namespace algos
