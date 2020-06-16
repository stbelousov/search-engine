cmake_minimum_required(VERSION 3.13)

project(search_engine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

set(SOURCES
    src/dict.cpp
    src/header.cpp
    src/index.cpp
    src/postings.cpp
    src/ranking.cpp
    src/utils.cpp
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O3")

file(COPY scripts/parser.py DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(indexer  src/indexer.cpp  ${SOURCES})
add_executable(searcher src/searcher.cpp ${SOURCES})
