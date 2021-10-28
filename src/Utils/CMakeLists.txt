# Generic test that uses conan libs
add_library(Utils
        private/Utils.cpp

)
target_link_libraries(
        Utils
        PRIVATE
        project_options
        project_warnings
        CONAN_PKG::fmt
        CONAN_PKG::spdlog
        CONAN_PKG::imgui-sfml
        CONAN_PKG::boost
)
target_include_directories(Utils PUBLIC "${PROJECT_BINARY_DIR}") #for access config.h