#pragma once
#include <string>
#include <vector>

#include "frysGL/shader/Shader.h"
#include "glm/vec3.hpp"

namespace frysgl
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec3 TexCoords;
    };

    struct Texture
    {
        unsigned int Id;
        std::string Type;
    };

    class Mesh
    {
    public:
        // mesh data
        std::vector<Vertex>       m_Vertices;
        std::vector<unsigned int> m_Indices;
        std::vector<Texture>      m_Textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader &shader);

    private:
        //  render data
        unsigned int VAO, VBO, EBO;
        void setupMesh();
    };
} // frysgl