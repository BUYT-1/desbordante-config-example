#include "CSVParser.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <utility>
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
    //Wrong path
    if (!source_) {
        throw std::runtime_error("Error: couldn't find file " + path.string());
    }
    if (separator == '\0') {
        throw std::invalid_argument("Invalid separator");
    }
    if (has_header) {
        GetNext();
    } else {
        PeekNext();
    }

    std::vector<std::string> next_parsed = GetNextRow();
    number_of_columns_ = next_parsed.size();
    column_names_ = std::move(next_parsed);

    if (!has_header) {
        for (int i = 0; i < number_of_columns_; i++) {
            column_names_[i] = std::to_string(i);
        }
    }
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

}

std::vector<std::string> CSVParser::ParseString(const std::string& s) const {

}

std::vector<std::string> CSVParser::ParseLine(const unsigned long long line_index) {

}

std::vector<std::string> CSVParser::GetNextRow() {
    return {};
}

