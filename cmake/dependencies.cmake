function(add_dependency)
    cmake_parse_arguments(
        DEPS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "TARGET;VCPKG_PKG" # list of names of mono-valued arguments
        "LIB;VCPKG_LIB" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
    # note: if it remains unparsed arguments, here, they can be found in variable PARSED_ARGS_UNPARSED_ARGUMENTS
    if(NOT DEPS_TARGET)
        message(FATAL_ERROR "You must provide a target")
    endif(NOT DEPS_TARGET)

    if (DEFINED VCPKG_ROOT)
      find_package(${DEPS_VCPKG_PKG} CONFIG REQUIRED)
      target_link_libraries(${DEPS_TARGET}
        PRIVATE
          ${DEPS_VCPKG_LIB}
      )
    else()
      target_link_libraries(${DEPS_TARGET}
        PRIVATE
          ${DEPS_LIB}
      )
    endif()
endfunction(add_dependency)