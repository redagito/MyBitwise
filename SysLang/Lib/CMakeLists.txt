﻿file (GLOB_RECURSE SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

add_library (${PROJECT_NAME}Lib ${SOURCE_FILES})

set_target_properties (${PROJECT_NAME}Lib PROPERTIES 
	C_STANDARD 11
)

target_include_directories (${PROJECT_NAME}Lib
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)