#pragma once

#include <functional>
#include <utility>
#include "Primitive.h"

#include "ProgramOptionStrings.h"
#include "Option.h"
#include "Descriptions.h"
#include "OptionTypes.h"

namespace algos {

class MetricVerifier : public Primitive {
private:
    config::EqNullsType is_null_equal_null_{};
    config::ParameterType parameter_{};
    config::NullDIstInfType dist_to_null_infinity_{};
    std::vector<unsigned int> lhs_indices_{};
    std::vector<unsigned int> rhs_indices_{};
    std::string metric_{};
    std::string algo_{};
    unsigned int q_{};

    bool processing_completed_ = false;

    void CheckIndices(std::vector<unsigned int> const& value) const;
    void SetExecuteOpts() override;
    void FitInternal(StreamRef input_generator) override;
    void AddPossibleOpts();

    static const config::OptionType<decltype(lhs_indices_)> LhsIndices;
    static const config::OptionType<decltype(rhs_indices_)> RhsIndices;
    static const config::OptionType<decltype(metric_)> Metric;
    static const config::OptionType<decltype(algo_)> Algo;
    static const config::OptionType<decltype(q_)> QGramLength;

public:
    MetricVerifier();
    unsigned long long Execute() override;
};

}  // namespace algos
