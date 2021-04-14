add_library(error OBJECT)
target_sources(error
    PUBLIC
    ssa_error.c
    ssa_stream.c
    ssa_strerror.c)

target_include_directories(error
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_compile_definitions(error
    PRIVATE
    $<BUILD_INTERFACE:SSA_BUILD_GUARDS>)

install(FILES ssa_error.h
    DESTINATION ${CUSTOM_INCLUDE_DIR}/error)