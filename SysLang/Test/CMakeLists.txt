file (GLOB_RECURSE SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

add_executable (${PROJECT_NAME}Test ${SOURCE_FILES})

set_target_properties (${PROJECT_NAME}Test PROPERTIES 
	C_STANDARD 11
)

target_include_directories (${PROJECT_NAME}Test
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries (${PROJECT_NAME}Test
	PRIVATE ${PROJECT_NAME}Lib
)