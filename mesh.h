#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

class Mesh
{
    public:
        Mesh();

        void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
        void RenderMesh();
        void ClearMesh();

        virtual ~Mesh();

    protected:

    private:
        GLuint VBO, VAO, IBO;
        GLsizei indexCount;

};

#endif // MESH_H
