# CMake file for P4Emi.

include(common)

project(emi)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h" @ONLY
)
# Source files for emi.
set(
  EMI_SOURCES
  emi.cpp
)

set(
  EMI_LIBS
  p4tools-common
  testgen
  inja
)

file(GLOB tools_targets RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/targets ${CMAKE_CURRENT_SOURCE_DIR}/targets/*)
foreach(ext ${tools_targets})
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/targets/${ext}/CMakeLists.txt)
    # Generate an option that makes it possible to disable this extension.
    string(MAKE_C_IDENTIFIER ${ext} EXT_AS_IDENTIFIER)
    string(TOUPPER ${EXT_AS_IDENTIFIER} EXT_AS_OPTION_NAME)
    string(CONCAT ENABLE_EXT_OPTION "ENABLE_TOOLS_TARGET_" ${EXT_AS_OPTION_NAME})
    string(CONCAT EXT_HELP_TEXT "Build the " ${ext} " target")
    option(${ENABLE_EXT_OPTION} ${EXT_HELP_TEXT} ON)
    if(${ENABLE_EXT_OPTION})
      message("-- Enabling target ${ext}")
      add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/targets/${ext})
      set(include_statements_var "${include_statements_var}#include \"backends/p4tools/modules/emi/targets/${ext}/register.h\"\n")
      set(compiler_targets "${compiler_targets}    ${ext}RegisterCompilerTarget();\n")
    endif()
  endif()
endforeach(ext)

# Convert the list of files into #includes
foreach(include_file ${include_files})
endforeach()

# Fill the template
configure_file(register.h.in register.h)

add_p4tools_library(emi ${EMI_SOURCES})
target_link_libraries(emi p4tools-common testgen inja)

add_p4tools_executable(p4emi main.cpp)

target_link_libraries(
  p4emi
  emi
  ${EMI_LIBS}
  ${P4C_LIBRARIES}
  ${P4C_LIB_DEPS}
  ${CMAKE_THREAD_LIBS_INIT}
)

add_custom_target(
  linkemi
  # Add some convenience links for invoking emi.
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/emi ${CMAKE_BINARY_DIR}/emi
)

add_dependencies(p4emi linkemi)
