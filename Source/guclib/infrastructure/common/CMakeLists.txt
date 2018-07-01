CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(guclib.infrastructure.common)

AUX_SOURCE_DIRECTORY(source PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})

# on Linux
TARGET_LINK_LIBRARIES(${PROJECT_NAME} guclib.infrastructure.linux)

# on Windows
# something else
