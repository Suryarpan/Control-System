function(BuildOutOfSource)
    # Get full path
    get_filename_component(srcdir ${CMAKE_SOURCE_DIR} REALPATH)
    get_filename_component(bindir ${CMAKE_BINARY_DIR} REALPATH)

    if (${srcdir} STREQUAL ${bindir})
        message("*#####################################################################*")
        message("*#               WARNING: in-source Build is not allowed             #*")
        message("*# Please create a separate BUILD directory and run CMake from there #*")
        message("*#####################################################################*")
        message(FATAL_ERROR "Quiting Configuration")
    endif ()
endfunction(BuildOutOfSource)

BuildOutOfSource()
