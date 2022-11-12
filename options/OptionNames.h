#pragma once
#include <string>

namespace opt_names {
const std::string_view kHelp = "help";
const std::string_view kTask = "task";
const std::string_view kAlgorithm = "algorithm";
const std::string_view kData = "data";
const std::string_view kSeparatorConfig = "separator";
const std::string_view kHasHeader = "has_header";
const std::string_view kEqualNulls = "is_null_equal_null";
const std::string_view kThreads = "threads";
const std::string_view kError = "error";
const std::string_view kMaximumLhs = "max_lhs";
const std::string_view kSeed = "seed";
const std::string_view kMinimumSupport = "minsup";
const std::string_view kMinimumConfidence = "minconf";
const std::string_view kInputFormat = "input_format";
const std::string_view kTIdColumnIndex = "tid_column_index";
const std::string_view kItemColumnIndex = "item_column_index";
const std::string_view kFirstColumnTId = "has_tid";
const std::string_view kMetric = "metric";
const std::string_view kLhsIndices = "lhs_indices";
const std::string_view kRhsIndices = "rhs_indices";
const std::string_view kParameter = "parameter";
const std::string_view kDistToNullIsInfinity = "dist_to_null_infinity";
const std::string_view kQGramLength = "q";
const std::string_view kMetricAlgorithm = "metric_algorithm";
const std::string_view kRadius = "radius";
const std::string_view kRatio = "ratio";
}  // namespace program_option_strings
