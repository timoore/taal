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
  shading/ComputeShaderSet.cpp
  shading/HDR.cpp
  sky/StarCatalog.cpp
  sky/StarField.cpp
  util/Component.cpp
  util/DeviceProperties.cpp
  util/FrameGraph.cpp
)

set(TAAL_PUBLIC_HEADERS
  shading/ComputeShaderSet.cpp
  shading/HDR.cpp
  shading/shaderDefs.h
  sky/StarCatalog.h
  sky/StarField.h
  util/Component.h
  util/DeviceProperties.h
  util/FrameGraph.h
  util/math.h
)

set_target_properties(
  taal
  PROPERTIES
  PUBLIC_HEADER "${TAAL_PUBLIC_HEADERS}"
)

target_link_libraries(taal PUBLIC vsg::vsg)
target_include_directories(taal PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
target_include_directories(taal PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

configure_file("Config.h.in"
  "${PROJECT_BINARY_DIR}/include/taal/Config.h")

install(TARGETS taal
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/taal")
