option(TAAL_TESTS_ENABLED "Whether to enable tests" ON)

if(TAAL_TESTS_ENABLED)
  list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()

cmake_minimum_required(VERSION 3.27)

project(taal
  VERSION 0.0
  DESCRIPTION "Nodes and classes for VSG"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options( -Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Wall -Wextra -Wparentheses -Wreturn-type -Wmissing-braces -Wunknown-pragmas -Wshadow -Wunused)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(vsg REQUIRED)

if(TAAL_TESTS_ENABLED)
  find_package(vsgXchange REQUIRED)
  find_package(vsgImGui)
endif()

add_subdirectory(src/taal)

