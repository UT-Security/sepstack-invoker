cmake_minimum_required(VERSION 3.13)

project(sepstack-invoker
        VERSION 0.1
        DESCRIPTION "Invoke a function on a separate stack")

enable_language(C CXX ASM)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies ###################

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/Catch2/src)
    message(FATAL_ERROR "Can't find Catch2. Run git submodule update --init.")
endif ()

add_subdirectory("${CMAKE_SOURCE_DIR}/Catch2/")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Catch2/contrib")

# Tests executable ###################

file(GLOB test_SRC CONFIGURE_DEPENDS "test_*.cpp")
add_executable(sepstack-invoker-test ${test_SRC} sepstack_trampoline.S)
target_link_libraries(sepstack-invoker-test Catch2::Catch2WithMain)
add_test(Unit-Tests ${CMAKE_BINARY_DIR}/sepstack-invoker-test)

enable_testing()