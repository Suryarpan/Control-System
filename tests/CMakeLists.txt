################################################################################
#                         GoogleTest Setups for CMake                          #
################################################################################
mark_as_advanced(BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS gmock_build_tests
        gtest_build_samples gtest_build_tests gtest_disable_pthreads
        gtest_force_shared_crt gtest_hide_internal_symbols)

function(add_to_gtest EXEC_NAME LIB_NAME)
    add_executable(${EXEC_NAME} ${EXEC_NAME}.cpp)
    target_link_libraries(${EXEC_NAME}
            PUBLIC
            gtest gmock gtest_main
            PRIVATE
            spacial_flags
            ${LIB_NAME})
    gtest_discover_tests(${EXEC_NAME})
endfunction()

include(FetchContent)
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.10.0)
FetchContent_MakeAvailable(googletest)

################################################################################
#                              The Tests to build                              #
################################################################################
