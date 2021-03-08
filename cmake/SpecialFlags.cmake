# openmp for multithreading
option (ENABLE_OPENMP "Use OpenMP for multithreading" OFF)

if(ENABLE_OPENMP)
    find_package(OpenMP)
endif(ENABLE_OPENMP)
if(OpenMP_FOUND)
    set(HAVE_OPENMP true)
endif(OpenMP_FOUND)

# cpu intrinsics related
include(CheckCCompilerFlag)
list(APPEND INTRIN_FLAG "")

# SSE
option (ENABLE_SSE "Compile with SSE instruction set support" OFF)
if(ENABLE_SSE)
    foreach(FLAG " -msse" " /arch:SSE")
        unset(HAVE_SSE CACHE)
        check_c_compiler_flag(${FLAG} HAVE_SSE)
        if(HAVE_SSE)
            list(APPEND INTRIN_FLAG ${FLAG})
            set(ALIGNMENT 16)
            break()
        endif(HAVE_SSE)
    endforeach(FLAG " -msse" " /arch:SSE")
endif(ENABLE_SSE)

# SSE2
option (ENABLE_SSE2 "Compile with SSE2 instruction set support" OFF)
if(ENABLE_SSE2)
    foreach(FLAG " -msse2" " /arch:SSE2")
        unset(HAVE_SSE2 CACHE)
        check_c_compiler_flag(${FLAG} HAVE_SSE2)
        if(HAVE_SSE2)
            set(ALIGNMENT 16)
            list(APPEND INTRIN_FLAG ${FLAG})
            break()
        endif(HAVE_SSE2)
    endforeach(FLAG " -msse2" " /arch:SSE2")
endif(ENABLE_SSE2)

# AVX
option (ENABLE_AVX "Compile with AVX instruction set support" OFF)
if(ENABLE_AVX)
    foreach(FLAG " -mavx" " /arch:AVX")
        unset(HAVE_AVX CACHE)
        check_c_compiler_flag(${FLAG} HAVE_AVX)
        if(HAVE_AVX)
            set(ALIGNMENT 32)
            list(APPEND INTRIN_FLAG ${FLAG})
            break()
        endif(HAVE_AVX)
    endforeach(FLAG " -mavx" " /arch:AVX")
endif(ENABLE_AVX)

# AVX2
option (ENABLE_AVX2 "Compile with AVX2 instruction set support" OFF)
if(ENABLE_AVX2)
    foreach(FLAG " -mavx2" " /arch:AVX2")
        unset(HAVE_AVX2 CACHE)
        check_c_compiler_flag(${FLAG} HAVE_AVX2)
        if(HAVE_AVX2)
            set(ALIGNMENT 32)
            list(APPEND INTRIN_FLAG ${FLAG})
            break()
        endif(HAVE_AVX2)
    endforeach(FLAG " -mavx2" " /arch:AVX2")
endif(ENABLE_AVX2)

if (ENABLE_AVX2)
  foreach (FLAG "-mfma" "/arch:FMA")
    unset (HAVE_FMA CACHE)
    check_c_compiler_flag (${FLAG} HAVE_FMA)
    if (HAVE_FMA)
      set (FMA_FLAG ${FLAG})
      break()
    endif ()
  endforeach ()
endif ()

# AVX512F
option (ENABLE_AVX512F "Compile with SSE2 instruction set support" OFF)
if(ENABLE_AVX512F)
    foreach(FLAG " -mavx512f" " /arch:AVX512F")
        unset(HAVE_AVX512 CACHE)
        check_c_compiler_flag(${FLAG} HAVE_AVX512F)
        if(HAVE_AVX512F)
            set(ALIGNMENT 64)
            list(APPEND INTRIN_FLAG ${FLAG})
            break()
        endif(HAVE_AVX512F)
    endforeach(FLAG " -mavx512f" " /arch:AVX512F")
endif(ENABLE_AVX512F)

if(HAVE_SSE OR HAVE_SSE2 OR HAVE_AVX OR HAVE_AVX2 OR HAVE_FMA OR HAVE_AVX512F)
    set(HAVE_SIMD true)
endif(HAVE_SSE OR HAVE_SSE2 OR HAVE_AVX OR HAVE_AVX2 OR HAVE_FMA OR HAVE_AVX512F)

# Interface library for target
add_library(special_flags INTERFACE)

target_compile_options(special_flags
    INTERFACE
    ${INTRIN_FLAG})
if(HAVE_OPENMP)
    target_link_libraries(special_flags
    INTERFACE
    OpenMP::OpenMP_C)
endif(HAVE_OPENMP)
