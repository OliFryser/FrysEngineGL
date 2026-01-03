#include <frysGL/buffer/VertexBufferObject.h>

VertexBufferObject::VertexBufferObject() {
    glGenBuffers(1, &m_Handle);
}

void VertexBufferObject::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
}

void VertexBufferObject::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::SetBufferData(const void *data, const unsigned int datasize, const GLenum usage) {
    glBufferData(GL_ARRAY_BUFFER, datasize, data, usage);
}

