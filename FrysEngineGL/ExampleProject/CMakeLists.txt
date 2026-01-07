
file(GLOB_RECURSE target_inc "*.h" )
file(GLOB_RECURSE target_src "*.cpp" )

# Add source to this project's executable.
add_executable (ExampleProject ${target_src} "glad.c")

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
  set_property(TARGET ExampleProject PROPERTY CXX_STANDARD 23)
endif()

target_link_libraries(
	ExampleProject
	glfw3
	opengl32
	frysgl)

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")