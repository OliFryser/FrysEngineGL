
file(GLOB_RECURSE target_inc "*.h" )
file(GLOB_RECURSE target_src "*.cpp" )

# Add source to this project's executable.
add_executable (ExampleProject main.cpp)

target_link_libraries(ExampleProject PUBLIC
	glfw
	opengl32
	frysgl
	stb
)

target_compile_features(ExampleProject PUBLIC cxx_std_23)
set_target_properties(ExampleProject PROPERTIES CXX_EXTENSIONS OFF)