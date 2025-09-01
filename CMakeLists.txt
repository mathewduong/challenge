cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    acacia-challenge
    VERSION 0.1.0
    DESCRIPTION "ACACIA Systems Coding Challenge"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    acacia-challenge_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    acacia-challenge_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(acacia-challenge_lib PUBLIC cxx_std_17)

find_package(fmt REQUIRED)
target_link_libraries(acacia-challenge_lib PRIVATE fmt::fmt)

# ---- Declare executable ----

add_executable(acacia-challenge_exe source/main.cpp)
add_executable(acacia-challenge::exe ALIAS acacia-challenge_exe)

set_property(TARGET acacia-challenge_exe PROPERTY OUTPUT_NAME acacia-challenge)

target_compile_features(acacia-challenge_exe PRIVATE cxx_std_17)

target_link_libraries(acacia-challenge_exe PRIVATE acacia-challenge_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT acacia-challenge_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of acacia-challenge"
  )
endif()

include(cmake/dev-mode.cmake)
