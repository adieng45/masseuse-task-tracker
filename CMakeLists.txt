cmake_minimum_required(VERSION 3.16)
project(masseuse_task_tracker VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(masseuse_tracker src/main.cpp src/task_tracker.cpp)
target_include_directories(masseuse_tracker PRIVATE include)
target_compile_options(masseuse_tracker PRIVATE -Wall -Wextra -Wpedantic)

enable_testing()
add_executable(tracker_tests tests/task_tracker_tests.cpp src/task_tracker.cpp)
target_include_directories(tracker_tests PRIVATE include)
target_compile_options(tracker_tests PRIVATE -Wall -Wextra -Wpedantic)
add_test(NAME tracker_tests COMMAND tracker_tests)
