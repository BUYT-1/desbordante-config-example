cmake_minimum_required(VERSION 3.24)
project(desbConfig)

set(CMAKE_CXX_STANDARD 17)

include_directories(options)

add_executable(desbConfig main.cpp Primitive.h IDatasetStream.h CSVParser.h CSVParser.cpp
        Metric.h Metric.cpp options/Descriptions.h options/ProgramOptionStrings.h options/Util.h
        options/OptionTypes.h Primitive.cpp options/IOption.h)
