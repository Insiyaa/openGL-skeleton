#include "shader.h"

Shader::Shader()
{
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
    CompileShaders(vertexCode, fragmentCode);
}

void Shader::CompileShaders(const char* vertexCode, const char* fragmentCode)
{
    shaderID =  glCreateProgram();

    if (!shaderID)
    {
        std::cerr << "Error creating shader!\n";
        return;
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        std::cerr << "Error Linking: " << eLog << std::endl;
        return;
    }

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        std::cerr << "Error Validating: " << eLog << std::endl;
        return;
    }

    // get uniform variable ID
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformView = glGetUniformLocation(shaderID, "view");
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        std::cerr << "Error Compiling: " << shaderType << eLog << std::endl;
        return;
    }
    glAttachShader(theProgram, theShader);
}

void Shader::UseShader()
{
    glUseProgram(shaderID);
}

std::string Shader::ReadShaderCodeFromFile(const char* shaderPath)
{
	std::string code;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		code = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Shader file " << shaderPath << " cannot be read" << std::endl;
	}

	return code;
}

void Shader::CreateFromFiles(const char* vShader, const char* fShader)
{
	std::string vShaderCode = ReadShaderCodeFromFile(vShader);
	std::string fShaderCode = ReadShaderCodeFromFile(fShader);

	CompileShaders(vShaderCode.c_str(), fShaderCode.c_str());
}

Shader::~Shader()
{
    //dtor
}
