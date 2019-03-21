PROJECT(gsclib.infrastructure.linux)

AUX_SOURCE_DIRECTORY(source PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
   gsclib.infrastructure.api
   CONAN_PKG::boost_filesystem
   CONAN_PKG::boost_range
   CONAN_PKG::boost_bimap
)
