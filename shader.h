#ifndef SHADER_H
#define SHADER_H
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>


class Shader
{
    public:
        Shader();

        void CreateFromString(const char* vertexCode, const char* fragmentCode);
        void CreateFromFiles(const char* vShader, const char* fShader);

        GLuint GetProjectionLocation()
        {
            return this->uniformProjection;
        }
        GLuint GetModelLocation()
        {
            return this->uniformModel;
        }
        GLuint GetViewLocation()
        {
            return this->uniformView;
        }

        void UseShader();

        void ClearShader();

        virtual ~Shader();

    protected:

    private:
        GLuint shaderID, uniformProjection, uniformModel, uniformView;

        std::string ReadShaderCodeFromFile(const char* shaderPath);

        void CompileShaders(const char* vertexCode, const char* fragmentCode);
        void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

#endif // SHADER_H
