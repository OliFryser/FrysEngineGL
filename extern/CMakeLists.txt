add_subdirectory(glad)
add_subdirectory("glm")
add_subdirectory("stb")
add_subdirectory(glfw)

# Header-only libraries
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/stb/include")
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/glm/include")