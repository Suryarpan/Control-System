cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

include(cmake/BuildOutOfSource.cmake)

project(SSA
    VERSION 0.1.0
    DESCRIPTION "A library to simulate system equation"
    LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED true)

if(MSVC)
    message(WARNING "Have never been tested under MSVC!")
endif(MSVC)

if(NOT APPLE)
    set(CMAKE_INSTALL_RPATH $ORIGIN) # rpath to be used
endif(NOT APPLE)

option(ENABLE_CODE_COVERAGE "Build with code coverage information" OFF)
if(ENABLE_CODE_COVERAGE)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU")
        set(COV_FLAG "--coverage")
    elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(COV_FLAG "--coverage")
    else()
        message(WARNING "Coverage flag unknown")
    endif(CMAKE_C_COMPILER_ID MATCHES "GNU")
    
    if(COV_FLAG)
    add_compile_options(${COV_FLAG})
    add_link_options(${COV_FLAG})
    endif()
endif(ENABLE_CODE_COVERAGE)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")
endif()

if(POLICY CMP0076)
    cmake_policy(SET CMP0076 NEW)
endif(POLICY CMP0076)

# Build as shared lib by default
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Build test suit
option(BUILD_TESTS "Build test suit" ON)

# Check required headers
include(CheckIncludeFile)
check_include_file(malloc.h HAVE_MALLOC_H)
check_include_file(stddef.h HAVE_STDDEF_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(stdarg.h HAVE_STDARG_H)
check_include_file(math.h HAVE_MATH_H)

# Check function prototype
include(CheckPrototypeDefinition)
check_prototype_definition(memalign "void *memalign(size_t alignment, size_t size)" "0" malloc.h HAVE_DECL_MEMALIGN)
check_prototype_definition (posix_memalign "int posix_memalign(void **memptr, size_t alignment, size_t size)" "0" stdlib.h HAVE_DECL_POSIX_MEMALIGN)

# Check symbol exists
include(CheckSymbolExists)
check_symbol_exists(memalign malloc.h HAVE_MEMALIGN)
check_symbol_exists(posix_memalign stdlib.h HAVE_POSIX_MEMALIGN)
check_symbol_exists(isnan math.h HAVE_ISNAN)
check_symbol_exists(abort stdlib.h HAVE_ABORT)

find_library(LIBM_LIBRARY NAMES m)
if(LIBM_LIBRARY)
    set(HAVE_LIBM true)
else(LIBM_LIBRARY)
    message(ERROR "Need math library")
endif(LIBM_LIBRARY)

# Configure the project config file
configure_file(ssa_config.h.in ssa_config.h)
include_directories(${CMAKE_BINARY_DIR})
# Installation
include(GNUInstallDirs)
set(CUSTOM_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR}/ssa)

# Project compile flags
set(CMAKE_POSITION_INDEPENDENT_CODE true)

include(cmake/ProjectWarnings.cmake)
set_project_warnings()
if(HAVE_SSE OR HAVE_SSE2)
    check_include_file(emmintrin.h HAVE_EMMINTRIN_H)
elseif(HAVE_AVX OR HAVE_AVX2 OR HAVE_FMA OR HAVE_AVX512F)
    check_include_file(immintrin.h HAVE_IMMINTRIN_H)
endif(HAVE_SSE OR HAVE_SSE2)

include(cmake/SpecialFlags.cmake)

# Project Building
add_subdirectory(error)
add_subdirectory(datatypes)

# Main library
add_library(ssa
    SHARED
    $<TARGET_OBJECTS:error>
    $<TARGET_OBJECTS:ssa_types>)

# Export header
include(GenerateExportHeader)
generate_export_header(ssa
    BASE_NAME SSA)

set_target_properties(ssa
    PROPERTIES
    SOVERSION ${SSA_VERSION_MAJOR}
    VERSION ${SSA_VERSION}
    PUBLIC_HEADER 
    ${CMAKE_SOURCE_DIR}/include/ssa.h)

install(TARGETS ssa 
    PUBLIC_HEADER 
    DESTINATION ${CUSTOM_INCLUDE_DIR}
    LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES
    ${CMAKE_BINARY_DIR}/ssa_export.h
    ${CMAKE_BINARY_DIR}/ssa_config.h
    DESTINATION ${CUSTOM_INCLUDE_DIR})

# CMake package Config File
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    SSAConfigVersion.cmake
    VERSION ${SSA_VERSION}
    COMPATIBILITY SameMajorVersion
)

if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
    if(BUILD_TESTS)
        enable_testing()
        add_subdirectory(tests)
    endif(BUILD_TESTS)
endif(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
