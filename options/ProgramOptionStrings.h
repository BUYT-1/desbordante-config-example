#pragma once
#include <string>

namespace opt_strings {
const std::string kHelp = "help";
const std::string kTask = "task";
const std::string kAlgorithm = "algorithm";
const std::string kData = "data";
const std::string kSeparatorConfig = "separator";
const std::string kHasHeader = "has_header";
const std::string kEqualNulls = "is_null_equal_null";
const std::string kThreads = "threads";
const std::string kError = "error";
const std::string kMaximumLhs = "max_lhs";
const std::string kSeed = "seed";
const std::string kMinimumSupport = "minsup";
const std::string kMinimumConfidence = "minconf";
const std::string kInputFormat = "input_format";
const std::string kTIdColumnIndex = "tid_column_index";
const std::string kItemColumnIndex = "item_column_index";
const std::string kFirstColumnTId = "has_tid";
const std::string kMetric = "metric";
const std::string kLhsIndices = "lhs_indices";
const std::string kRhsIndices = "rhs_indices";
const std::string kParameter = "parameter";
const std::string kDistToNullIsInfinity = "dist_to_null_infinity";
const std::string kQGramLength = "q";
const std::string kMetricAlgorithm = "metric_algorithm";
const std::string kRadius = "radius";
const std::string kRatio = "ratio";
}  // namespace program_option_strings
