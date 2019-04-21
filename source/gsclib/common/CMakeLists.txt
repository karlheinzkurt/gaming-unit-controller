PROJECT(gsclib.common)

AUX_SOURCE_DIRECTORY(source PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_link_libraries(${PROJECT_NAME} 
   gsclib.infrastructure.api 
   gsclib.api 
   gsclib.utility 
   ${Log4cxx_LIBRARIES}
   CONAN_PKG::cpprestsdk
)

