#pragma once
#include <glad/glad.h>

namespace frysgl
{
    class VertexArrayObject {
    private:
        GLuint m_Handle{};

    public:
        VertexArrayObject();
        void Bind() const;

        static void Unbind();
    };
}