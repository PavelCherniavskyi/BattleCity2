add_executable(tests
        tests.cpp
        gtest_main.cpp
        "${CMAKE_SOURCE_DIR}/src/private/SpriteHolder.cpp"
        )
target_link_libraries(tests PRIVATE
        project_warnings
        project_options
        CONAN_PKG::imgui-sfml
        CONAN_PKG::spdlog
        CONAN_PKG::gtest
        )
target_include_directories(tests PUBLIC "${PROJECT_BINARY_DIR}" PUBLIC "${CMAKE_SOURCE_DIR}/src") #for access config.h and src directory

add_test(NAME GTestMain COMMAND tests)