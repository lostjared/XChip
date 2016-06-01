if(${BUILD_SDL_PLUGINS} STREQUAL "ON")
	add_subdirectory(SDLPlugins)
endif()

if(${BUILD_SFML_PLUGINS} STREQUAL "ON")
	add_subdirectory(SFMLPlugins)
endif()


