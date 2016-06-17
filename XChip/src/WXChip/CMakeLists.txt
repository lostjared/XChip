
if( BUILD_WXCHIP )

	PROJECT(WXChip)
	# find and include wxWidgets dependencies
	FIND_PACKAGE(wxWidgets REQUIRED html adv core base net aui xrc qa richtext )
	INCLUDE(${wxWidgets_USE_FILE})
	
	FILE(GLOB SRC ./*.cpp)

	ADD_EXECUTABLE(${PROJECT_NAME}  ${SRC})
	ADD_DEPENDENCIES(${PROJECT_NAME} Core Utils)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${wxWidgets_LIBRARIES} Core Utils)

	INSTALL(TARGETS WXChip DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/WXChip)
endif()
