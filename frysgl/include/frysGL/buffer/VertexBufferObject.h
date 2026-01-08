#pragma once

#include <glad/glad.h>


class VertexBufferObject {
private:
    GLuint m_Handle{};

public:
    VertexBufferObject();
    void Bind() const;

    static void Unbind();

    static void SetBufferData(const void* data, unsigned int datasize, GLenum usage);
};