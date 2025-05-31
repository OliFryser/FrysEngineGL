
file(GLOB_RECURSE target_inc "*.h" )
file(GLOB_RECURSE target_src "*.cpp" )

# Add source to this project's executable.
add_executable (ExampleProject ${target_src} "glad.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ExampleProject PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(ExampleProject
	glfw3
	opengl32)
