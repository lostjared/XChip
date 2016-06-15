project(Core)

# exceptions and rtti are NOT used on Core.
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")

file(GLOB_RECURSE SRC ./*.cpp)
file(GLOB_RECURSE HEADERS XChip/*.h)
add_library(${PROJECT_NAME} ${HEADERS} ${SRC})
target_link_libraries(${PROJECT_NAME} Utils)


INSTALL(TARGETS Core  DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib/)
