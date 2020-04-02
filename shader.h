#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <GL/glew.h>

class Shader
{
    public:
        // Load shader programs from different files, build them for GPU and send build files to GPU
        Shader(const std::string& fileName);
        // It's going to set the GPU in a state that it's using vertex and fragment shader function
        void Bind();
        virtual ~Shader();


    protected:

    private:
        // 2 for fragment and vertex shaders, 3 if using geometric shader too
        static const unsigned int NUM_SHADER = 2;
        Shader(const Shader& other){}
        void operator=(const Shader& other){}
        // openGl's way of keeping track where the program is
        GLuint m_program;
        GLuint m_shaders[NUM_SHADER];
};

#endif // SHADER_H
