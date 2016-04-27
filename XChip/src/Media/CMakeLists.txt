
if(${USING_SDL_MEDIA} STREQUAL "ON")
	add_subdirectory(SDLMedia)
	
endif()

if(${USING_WX_MEDIA} STREQUAL "ON")
	add_subdirectory(WXMedia)
	
endif()


