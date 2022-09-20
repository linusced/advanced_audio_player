#include "lc_gui/shader.hpp"

lc_gui::Shader::Shader(const std::string &vertexCode, const std::string &fragmentCode)
{
    std::string frag = fragmentCode.empty() ? "#version 330 core void main(){}" : fragmentCode;

    const char *vertex_c_str = vertexCode.c_str(),
               *fragment_c_str = fragmentCode.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_c_str, 0);
    glCompileShader(vertexShader);
    compileErrors(vertexShader, "VERTEX");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_c_str, 0);
    glCompileShader(fragmentShader);
    compileErrors(fragmentShader, "FRAGMENT");

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);

    glLinkProgram(id);
    compileErrors(id, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

lc_gui::Shader::~Shader()
{
    glDeleteProgram(id);
}

void lc_gui::Shader::activate()
{
    glUseProgram(id);
}

void lc_gui::Shader::compileErrors(unsigned int shader, const std::string &type)
{
    int hasCompiled;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (!hasCompiled)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            throw std::runtime_error("lc_gui::Shader SHADER_COMPILATION_ERROR for: " + type + '\n' + infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (!hasCompiled)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            throw std::runtime_error("lc_gui::Shader SHADER_LINKING_ERROR for: " + type + '\n' + infoLog);
        }
    }
}