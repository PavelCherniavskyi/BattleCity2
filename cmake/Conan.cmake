set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES} imgui-sfml/2.1@bincrafters/stable)

macro(run_conan)
    # Download automatically, you can also just copy the conan.cmake file
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(===> "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                TLS_VERIFY ON)
    endif()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    conan_add_remote(NAME bincrafters URL https://bincrafters.jfrog.io/artifactory/api/conan/public-conan)

    conan_cmake_run(
            REQUIRES
            ${CONAN_EXTRA_REQUIRES}
            gtest/cci.20210126
            fmt/8.0.1
            spdlog/1.9.2
            boost/1.77.0
            OPTIONS
            BASIC_SETUP
            CMAKE_TARGETS # individual targets to link to
            BUILD
            missing
    )
endmacro()