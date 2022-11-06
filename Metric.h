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
    void SetExecOpts();
    void AddPossibleOpts();

    static config::OptionInfo<decltype(lhs_indices_)> LhsIndices;
    static config::OptionInfo<decltype(rhs_indices_)> RhsIndices;
    static config::OptionInfo<decltype(metric_)> Metric;
    static config::OptionInfo<decltype(algo_)> Algo;
    static config::OptionInfo<decltype(q_)> QGramLength;

public:
    MetricVerifier();
    void Fit(StreamRef input_generator) override;
    unsigned long long Execute() override;
};

}  // namespace algos
