# Add source to this project's executable.
add_executable (ExampleProject main.cpp)

target_link_libraries(ExampleProject PRIVATE
	glfw
	frysgl
	stb
)

target_compile_features(ExampleProject PUBLIC cxx_std_23)
set_target_properties(ExampleProject PROPERTIES CXX_EXTENSIONS OFF)

add_custom_command(
		TARGET ExampleProject POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
				${CMAKE_CURRENT_LIST_DIR}/assets
				$<TARGET_FILE_DIR:ExampleProject>/assets
)