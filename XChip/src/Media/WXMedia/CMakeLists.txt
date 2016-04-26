project(WXMedia)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

FIND_PACKAGE(wxWidgets REQUIRED html adv core base net aui xrc qa richtext )
INCLUDE(${wxWidgets_USE_FILE})

file(GLOB_RECURSE SRC ./*.cpp)
file(GLOB_RECURSE HEADERS XChip/*.h)

add_library(${PROJECT_NAME} ${HEADERS} ${SRC})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} Core Utility)
