project(gsclib.test)

aux_source_directory(source PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_link_libraries(${PROJECT_NAME} 
   gsclib.infrastructure.mock 
   gsclib.api 
   gsclib.utility
   gsclib.common 
   CONAN_PKG::cpprestsdk
   CONAN_PKG::gtest
   ${Log4cxx_LIBRARIES}
)
