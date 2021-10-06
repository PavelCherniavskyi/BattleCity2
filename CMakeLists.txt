cmake_minimum_required(VERSION 3.12)

# Set the project name to your project name, my project isn't very descriptive
project(BattleCity2)

include(cmake/StandardProjectSettings.cmake)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)
enable_sanitizers(project_options)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(ENABLE_TESTING "Enable Test Builds" ON)
option(ENABLE_FUZZING "Enable Fuzzing Builds" OFF)

target_precompile_headers(project_options INTERFACE <vector> <string> <map> <utility> <memory> <variant> <spdlog/spdlog.h> <array> <unordered_map>)

include(cmake/Conan.cmake)
run_conan()

if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(test)
    message(===> "Tests are enabled")
endif()

add_subdirectory(src)

configure_file (config.h.in config.h @ONLY)