#pragma once

#include <glad/glad.h>

#include <span>


class VertexBufferObject {
private:
    GLuint m_Handle{};

public:
    VertexBufferObject();
    void Bind() const;

    static void Unbind();

    static void SetBufferData(const void* data, unsigned int datasize, GLenum usage);
};