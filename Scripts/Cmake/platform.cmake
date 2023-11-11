message(STATUS "platform: Initializing")

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if (WIN64)
	message(STATUS "platform: building on Windows")
	add_compile_options(-mwindows -Wl,--subsystem=windows)
endif ()

if(LINUX) 
    message(STATUS "platform: building on Linux")
endif()
