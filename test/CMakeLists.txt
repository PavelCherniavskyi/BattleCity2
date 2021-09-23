

add_library(gtest_main STATIC gtest_main.cpp)
target_link_libraries(gtest_main PUBLIC CONAN_PKG::gtest)
target_link_libraries(gtest_main PRIVATE project_options)

add_executable(tests
        tests.cpp
        "${CMAKE_SOURCE_DIR}/src/private/SpriteHolder.cpp"
        "${CMAKE_SOURCE_DIR}/src/private/Enums.cpp"
        )
target_link_libraries(tests PRIVATE
        project_warnings
        project_options
        gtest_main
        CONAN_PKG::imgui-sfml
        CONAN_PKG::spdlog
        )
target_include_directories(tests PUBLIC "${PROJECT_BINARY_DIR}" PUBLIC "${CMAKE_SOURCE_DIR}/src") #for access config.h and src directory

add_test(NAME GTestMain COMMAND tests)