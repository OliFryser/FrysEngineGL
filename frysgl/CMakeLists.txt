add_library(frysgl STATIC
		include/frysGL/buffer/VertexArrayObject.h src/buffer/VertexArrayObject.cpp
		include/frysGL/buffer/VertexBufferObject.h src/buffer/VertexBufferObject.cpp
		include/frysGL/camera/Camera.h src/camera/Camera.cpp
		include/frysGL/shader/Shader.h src/shader/Shader.cpp
)

target_include_directories(frysgl PUBLIC include)
target_compile_features(frysgl PUBLIC cxx_std_23)

target_link_libraries(frysgl PUBLIC glad glm)
