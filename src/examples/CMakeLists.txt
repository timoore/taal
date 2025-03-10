add_executable(skyviewer skyviewer.cpp)

target_compile_definitions(skyviewer PRIVATE vsgXchange_FOUND)
target_link_libraries(skyviewer PUBLIC taal vsgXchange::vsgXchange vsg::vsg)

install(TARGETS skyviewer
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
