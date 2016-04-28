if(${WXCHIP_BUILD} STREQUAL "ON")

	PROJECT(WXChip)
	# find and include wxWidgets dependencies
	FIND_PACKAGE(wxWidgets REQUIRED html adv core base net aui xrc qa richtext )
	INCLUDE(${wxWidgets_USE_FILE})
	

	FILE(GLOB_RECURSE SRC ./*.cpp)
	ADD_EXECUTABLE(${PROJECT_NAME}  ${SRC})
	ADD_DEPENDENCIES(${PROJECT_NAME} Core Utility)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${wxWidgets_LIBRARIES} Core Utility)




endif()
