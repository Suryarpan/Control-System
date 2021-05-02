add_library(ssa_types OBJECT)
target_sources(ssa_types
    PUBLIC
    polynom.c)

target_include_directories(ssa_types
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_compile_definitions(ssa_types
    PRIVATE
    $<BUILD_INTERFACE:SSA_BUILD_GUARD>)

install(FILES ssa_types.h
    DESTINATION ${CUSTOM_INCLUDE_DIR}/types)