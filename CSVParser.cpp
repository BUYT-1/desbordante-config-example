#include "CSVParser.h"

#include <filesystem>
#include <string>
#include <vector>

#include <boost/tokenizer.hpp>

inline std::string& CSVParser::rtrim(std::string& s) {
    return s;
}

CSVParser::CSVParser(const std::filesystem::path& path) : CSVParser(path, ',', true) {}

CSVParser::CSVParser(const std::filesystem::path& path, char separator, bool has_header)
        : source_(path),
          separator_(separator),
          has_header_(has_header),
          has_next_(true),
          next_line_(),
          number_of_columns_(),
          column_names_(),
          relation_name_(path.filename().string()) {
    // whatever
}

void CSVParser::GetNext() {

}

void CSVParser::PeekNext() {

}

void CSVParser::SkipLine() {

}

void CSVParser::Reset() {

}

void CSVParser::GetLine(const unsigned long long line_index) {

}

void CSVParser::GetNextIfHas() {

}

std::string CSVParser::GetUnparsedLine(const unsigned long long line_index) {
    return "";
}

std::vector<std::string> CSVParser::ParseString(const std::string& s) const {
    return {};
}

std::vector<std::string> CSVParser::ParseLine(const unsigned long long line_index) {
    return {};
}

std::vector<std::string> CSVParser::GetNextRow() {
    return {};
}

