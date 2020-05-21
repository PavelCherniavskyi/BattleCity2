function(enable_sanitizers project_name)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")

        option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" ON)
        if(ENABLE_COVERAGE)
            target_compile_options(${project_name} INTERFACE --coverage -O0 -g)
            target_link_libraries(${project_name} INTERFACE --coverage)
        endif()

        set(SANITIZERS "")

        option(ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ON)
        if(ENABLE_SANITIZER_ADDRESS)
            list(APPEND SANITIZERS "address")
        endif()

        option(ENABLE_SANITIZER_LEAK "Enable leak memory sanitizer" ON)
        if(ENABLE_SANITIZER_LEAK)
            list(APPEND SANITIZERS "leak")
        endif()

        option(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "Enable undefined behavior sanitizer" ON)
        if(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR)
            list(APPEND SANITIZERS "undefined")
        endif()

        option(ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
        if(ENABLE_SANITIZER_THREAD)
            list(APPEND SANITIZERS "thread")
        endif()

        list(JOIN SANITIZERS "," LIST_OF_SANITIZERS)
    else()
        message(STATUS "SANITIZERS compiler doesn't match")
    endif()

    if(LIST_OF_SANITIZERS)
        if(NOT "${LIST_OF_SANITIZERS}" STREQUAL "")
            target_compile_options(${project_name} INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
            target_link_libraries(${project_name} INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
            message(STATUS "SANITIZERS enabled: ${LIST_OF_SANITIZERS}")
        else()
            message(STATUS "LIST_OF_SANITIZERS is empty")
        endif()
    else()
        message(STATUS "SANITIZERS are NOT enabled")
    endif()

endfunction()