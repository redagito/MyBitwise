file (GLOB_RECURSE SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
)

add_executable (${PROJECT_NAME}Test ${SOURCE_FILES})

set_target_properties (${PROJECT_NAME}Test PROPERTIES 
	C_STANDARD 99
)

target_include_directories (${PROJECT_NAME}Test
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_link_libraries (${PROJECT_NAME}Test
	PRIVATE ${PROJECT_NAME}Lib
)