PROJECT(gsclib.api)

AUX_SOURCE_DIRECTORY(source PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_link_libraries(${PROJECT_NAME} 
   gsclib.infrastructure.api 
   CONAN_PKG::boost_property_tree
   CONAN_PKG::boost_bimap
)


