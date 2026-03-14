cmake_minimum_required(VERSION 3.16)
project(nse_scanner VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_library(scanner_core
    src/bhavcopy_downloader.cpp
    src/candlestick_detector.cpp
    src/bullish_scanner.cpp
)

target_include_directories(scanner_core PUBLIC include)

add_executable(nse_scanner_app src/main.cpp)
target_link_libraries(nse_scanner_app PRIVATE scanner_core)
