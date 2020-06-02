# Generic test that uses conan libs
add_executable(${PROJECT_NAME} main.cpp private/Game.cpp ResourceHolder.hpp)
target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        project_options
        project_warnings
        CONAN_PKG::docopt.cpp
        CONAN_PKG::fmt
        CONAN_PKG::spdlog
        CONAN_PKG::imgui-sfml
)
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}") #for access config.h