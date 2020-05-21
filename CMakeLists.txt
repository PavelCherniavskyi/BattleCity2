cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_COMPILER /usr/bin/clang-10)
set(CMAKE_CXX_COMPILER /usr/bin/clang++-10)

# Set the project name to your project name, my project isn't very descriptive
project(BattleCity)

include(cmake/StandardProjectSettings.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" ON)
option(ENABLE_FUZZING "Enable Fuzzing Builds" ON)

target_precompile_headers(project_options INTERFACE <vector> <string> <map> <utility>)

set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES} imgui-sfml/2.1@bincrafters/stable)

include(cmake/Conan.cmake)
run_conan()

if(ENABLE_TESTING)
    enable_testing()
    message( "Building Tests. Be sure to check out test/constexpr_tests for constexpr testing" )
    add_subdirectory(test)
endif()

if(ENABLE_FUZZING)
    message( "Building Fuzz Tests, using fuzzing sanitizer https://www.llvm.org/docs/LibFuzzer.html" )
    add_subdirectory(fuzz_test)
endif()

add_subdirectory(src)