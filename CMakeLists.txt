cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(hw2)

include(lib/gtest.cmake)

find_package(Threads REQUIRED)

# Set compile flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unused-parameter")

include_directories(include)
### Library for weeDB
add_library(wee_db src/BaseOperator.cpp src/DBData.cpp src/OperatorsColumnar.cpp src/OperatorsVolcano.cpp src/OperatorsVector.cpp)

### Tests
add_executable(test_all test/WeeDB.cpp)
target_link_libraries(test_all wee_db gtest gtest_main pthread)

### Performance Measurements
add_executable(weedb src/WeeDB.cpp)
target_link_libraries(weedb wee_db)
