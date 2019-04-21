project(gsc)

aux_source_directory(source PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")
file(GLOB PROJECT_ETC "etc/*")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE} ${PROJECT_INCLUDES} ${PROJECT_ETC})
target_link_libraries(${PROJECT_NAME} 
   gsclib.infrastructure.api 
   gsclib.infrastructure.common 
   gsclib.common 
   CONAN_PKG::boost_program_options
   ${Log4cxx_LIBRARIES}
)

add_custom_target(${PROJECT_NAME}.Copy ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/etc $<TARGET_FILE_DIR:${PROJECT_NAME}>/etc
   DEPENDS ${PROJECT_NAME}
   COMMENT "Copying configuration files")

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES etc/gsc.logger.xml etc/gsc.config.xml DESTINATION /usr/share/gsc/)
install(FILES etc/gsc PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE DESTINATION /etc/init.d/)
install(DIRECTORY DESTINATION /var/run/gsc/) 

