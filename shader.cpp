#include "shader.h"
#include <iostream>
#include <fstream>


// helper functions for I/O
// Load files off the harddrive
static std::string LoadShader(const std::string& fileName);
// gives error msgs
void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static GLuint CreateShader(const std::string& text, GLenum shaderType);


Shader::Shader(const std::string& fileName)
{
    m_program = glCreateProgram();
    // vertex shader
    m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
    // fragment shader
    m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);
    // no standard for file ext, you can keep anything

    // add shaders to the shader program
    for (unsigned int i=0; i < NUM_SHADER; i++)
        glAttachShader(m_program, m_shaders[i]);

    // what part of data to read
    glBindAttribLocation(m_program, 0, "position");

    // link and validate shader
    glLinkProgram(m_program);
    CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Program linking failed.");

    glValidateProgram(m_program);
    CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid.");


}

Shader::~Shader()
{
    for (unsigned int i=0; i < NUM_SHADER; i++)
    {
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }
    glDeleteProgram(m_program);
}

void Shader::Bind()
{
    glUseProgram(m_program);
}

static GLuint CreateShader(const std::string& text, GLenum shaderType)
{
    GLuint shader =  glCreateShader(shaderType);
    if (shader == 0)
        std::cerr << "Error: shader creation failed" << std::endl;
    const GLchar* shaderSourceStrings[1];
    shaderSourceStrings[0] = text.c_str();
    GLint shaderSourceLengths[1];

    shaderSourceLengths[0] = text.length();

    glShaderSource(shader, 1, shaderSourceStrings, shaderSourceLengths);
    glCompileShader(shader);

    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed.");

    return shader;
}

static std::string LoadShader(const std::string& fileName)
{
    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
			output.append(line + "\n");
        }
    }
    else
    {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
    }

    return output;
}

void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
}