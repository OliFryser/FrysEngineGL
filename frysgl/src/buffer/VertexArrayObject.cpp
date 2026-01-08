#include <frysGL/buffer/VertexArrayObject.h>
namespace frysgl
{
    VertexArrayObject::VertexArrayObject()
    {
        glGenVertexArrays(1, &m_Handle);
    }

    void VertexArrayObject::Bind() const {
        glBindVertexArray(m_Handle);
    }

    void VertexArrayObject::Unbind() {
        glBindVertexArray(0);
    }
}