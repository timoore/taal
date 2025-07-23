set(SHADER_FILES
  shaders/stars.frag
  shaders/stars.vert)

install(FILES ${SHADER_FILES}
  DESTINATION ${TAAL_DATA_DIR}/shaders)
