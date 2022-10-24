#pragma once

#include <functional>
#include <utility>
#include "Primitive.h"

#include "ProgramOptionStrings.h"
#include "Option.h"
#include "Descriptions.h"
#include "OptParameter.h"

enum class Metric {
    euclidean = 0,
    levenshtein,
    cosine
};

enum class MetricAlgo {
    brute = 0,
    approx,
    calipers
};

namespace algos {

class MetricVerifier : public Primitive {
    // Note: it may make sense to split everything related to configuration
    // into some other class.
private:
    using OptLhsType = config::Option<std::vector<unsigned int>, MetricVerifier, &program_option_strings::kLhsIndices,
    &algos::config::descriptions::kDLhsIndices>;
    using OptMetricType = config::Option<std::string, MetricVerifier, &program_option_strings::kMetric,
    &algos::config::descriptions::kDMetric>;
    using OptRhsType = config::Option<std::vector<unsigned int>, MetricVerifier, &program_option_strings::kRhsIndices,
    &algos::config::descriptions::kDRhsIndices>;
    using OptMetricAlgoType = config::DefOption<std::string, MetricVerifier, &program_option_strings::kMetricAlgorithm,
    &algos::config::descriptions::kDMetricAlgorithm>;
    using OptQType = config::DefOption<unsigned int, MetricVerifier, &program_option_strings::kQGramLength,
    &algos::config::descriptions::kDQGramLength>;

    struct OptLhsIndices : public config::Option<std::vector<unsigned int>, MetricVerifier, &program_option_strings::kLhsIndices,
            &algos::config::descriptions::kDLhsIndices> {
        void Set(MetricVerifier& primitive, std::vector<unsigned int> value) override;
    };

    struct OptMetric : public OptMetricType {
        void Set(MetricVerifier& primitive, std::string value) override;
    };

    struct OptRhsIndices : public OptRhsType {
        void Set(MetricVerifier& primitive, std::vector<unsigned int> value) override;
    };

    struct OptMetricAlgo : public OptMetricAlgoType {
        OptMetricAlgo() : OptMetricAlgoType("brute") {}

        void Set(MetricVerifier & primitive, std::string value) override;
    };

    struct OptQ : public OptQType {
        OptQ() : OptQType(2) {}

        void Set(MetricVerifier& primitive, unsigned int value) override;
    };

    config::DefOption<bool, MetricVerifier, &program_option_strings::kEqualNulls,
    &algos::config::descriptions::kDEqualNulls> is_null_equal_null_{true};
    config::OptParameter<MetricVerifier> parameter_;
    config::DefOption<bool, MetricVerifier, &program_option_strings::kDistToNullIsInfinity,
    &algos::config::descriptions::kDDistToNullIsInfinity> dist_to_null_infinity_{false};
    OptLhsIndices lhs_indices_;
    OptRhsIndices rhs_indices_;
    OptMetric metric_;
    OptMetricAlgo algo_;
    OptQ q_;

    //bool metric_fd_holds_ = false;

    // std::shared_ptr<model::ColumnLayoutTypedRelationData> typed_relation_;
    // std::shared_ptr<ColumnLayoutRelationData> relation_; // temporarily parsing twice

    //bool CompareNumericValues(util::PLI::Cluster const& cluster) const;
    //bool CompareStringValues(util::PLI::Cluster const& cluster,
    //                         DistanceFunction const& dist_func) const;

public:
    MetricVerifier();

    void Fit(StreamRef input_generator) override;

    unsigned long long Execute() override;

    bool SetOption(std::string const& option_name, std::any const& value) override;

    bool SetOption(std::string const& option_name) override;

    [[nodiscard]] std::unordered_set<std::string> GetNeededOptions() const override;

    bool UnsetOption(std::string const& option_name) noexcept override;

private:
    bool processing_completed_ = false;
};

}  // namespace algos
