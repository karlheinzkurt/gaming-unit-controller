project(gsclib.test)

include_directories(${GTEST_INCLUDE_DIRS})

aux_source_directory(source PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_link_libraries(${PROJECT_NAME} gsclib.infrastructure.mock gsclib.common ${GTEST_BOTH_LIBRARIES} ${GMOCK_ALL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
