find_package(Python3 QUIET COMPONENTS Interpreter)

if(Python3_FOUND)
  add_custom_target(
    CreateStarCatalog
    COMMAND Python3::Interpreter
    ${CMAKE_CURRENT_SOURCE_DIR}/sky/CreateStarCatalog.py
    ${CMAKE_CURRENT_SOURCE_DIR}/sky/StarCatalog.json
    ${CMAKE_CURRENT_SOURCE_DIR}/sky/StarCatalog.cpp
  )
endif()

add_library(taal)

target_sources(taal
  PRIVATE
  sky/StarCatalog.cpp
  sky/StarField.cpp
)

set(TAAL_PUBLIC_HEADERS
  shading/shaderDefs.h
  sky/StarCatalog.h
  sky/StarField.h
  util/math.h
)

set_target_properties(
  taal
  PROPERTIES
  PUBLIC_HEADER "${TAAL_PUBLIC_HEADERS}"
)

target_link_libraries(taal PUBLIC vsg::vsg)
target_include_directories(taal PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
