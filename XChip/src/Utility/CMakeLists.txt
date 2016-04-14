project(Utility)
file(GLOB_RECURSE SRC ./*.cpp)
file(GLOB_RECURSE HEADERS XChip/*.h)

add_library(${PROJECT_NAME} ${HEADERS} ${SRC})
