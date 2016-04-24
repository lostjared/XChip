if(${USING_SDL_MEDIA} STREQUAL "ON")
	add_subdirectory(SDLMedia)
	
endif()

if(${WXCHIP_BUILD} STREQUAL "ON")
	add_subdirectory(WXMedia)
	
endif()


