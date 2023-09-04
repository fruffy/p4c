# Source files for mutate lib.
set(
  MUTATE_SOURCES
  ${MUTATE_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/tofino.cpp
  PARENT_SCOPE
)

# Override default settings in bf-p4c. We want those binaries to be built.
if (NOT TARGET p4cgraphs)
  set(ENABLE_P4C_GRAPHS ON CACHE BOOL "TRUE" FORCE)
  add_subdirectory(${P4C_SOURCE_DIR}/backends/graphs ${P4C_BINARY_DIR}/backends/graphs)
endif()

# Ideally, we'd link against just the Barefoot midend and a minimal set of
# transitive dependencies, but the compiler doesn't cleanly separate like
# that. Those transitive dependencies turn out to be most of the compiler!
set(
  MUTATE_LIBS
  tofinobackend
  bfn_p4runtime
  p4cgraphs
  # We do not use the target library variable here because top-level mutate will not find it.
  boost_system
  ${LIBDYNHASH_LIBRARY}
  ${MUTATE_LIBS}
  PARENT_SCOPE
)
