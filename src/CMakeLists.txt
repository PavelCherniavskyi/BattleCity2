# Generic test that uses conan libs
add_executable(${PROJECT_NAME}
        main.cpp
        private/Game.cpp
        private/SpriteHolder.cpp
        private/Player.cpp
        private/InputHandler.cpp
        private/FontHolder.cpp
)
target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        project_options
        project_warnings
        CONAN_PKG::fmt
        CONAN_PKG::spdlog
        CONAN_PKG::imgui-sfml
        CONAN_PKG::boost
)
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}") #for access config.h