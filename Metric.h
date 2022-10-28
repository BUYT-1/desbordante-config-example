#pragma once

#include <functional>
#include <utility>
#include "Primitive.h"

#include "ProgramOptionStrings.h"
#include "Option.h"
#include "Descriptions.h"
#include "OptParameter.h"
#include "OptNullEqualsNull.h"
#include "OptDistToNullInfinity.h"

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
    // into some other class. However, this would make everything more
    // complicated, which is not needed for now.
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
    using NullEqualsNullType = config::OptNullEqualsNull<MetricVerifier>;
    using ParameterType = config::OptParameter<MetricVerifier>;
    using DistToNullInfinityType = config::OptDistToNullInfinity<MetricVerifier>;

    struct OptLhsIndices : public OptLhsType {
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

    struct {
        NullEqualsNullType is_null_equal_null_;
        ParameterType parameter_;
        DistToNullInfinityType dist_to_null_infinity_;
        OptLhsIndices lhs_indices_;
        OptRhsIndices rhs_indices_;
        OptMetric metric_;
        OptMetricAlgo algo_;
        OptQ q_;
    } conf_metver_;

    NullEqualsNullType::Type is_null_equal_null_;
    ParameterType::Type parameter_;
    DistToNullInfinityType::Type dist_to_null_infinity_;
    OptLhsIndices::Type lhs_indices_;
    OptRhsIndices::Type rhs_indices_;
    OptMetric::Type metric_;
    OptMetricAlgo::Type algo_;
    OptQ::Type q_;

public:
    MetricVerifier();
    void Fit(StreamRef input_generator) override;
    unsigned long long Execute() override;
    bool SetOption(std::string const& option_name, std::any const& value) override;
    bool SetOption(std::string const& option_name) override;
    [[nodiscard]] std::unordered_set<std::string> GetNeededOptions() const override;
    bool UnsetOption(std::string const& option_name) noexcept override;

protected:
    void SetConfFields() override;

private:
    bool processing_completed_ = false;
};

}  // namespace algos
