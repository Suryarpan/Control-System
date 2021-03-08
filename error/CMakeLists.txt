add_library(error OBJECT)
target_sources(error
    PUBLIC
    ssa_error.h
    ssa_error.c
    ssa_log.c
    ssa_stream.c
    ssa_strerror.c)

target_include_directories(error
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    CMAKE_BINARY_DIR)