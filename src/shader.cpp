#include "shader.h"

Shader::Shader(const char** vs_source, const char** fs_source)
{
    unsigned int vertex = compile(GL_VERTEX_SHADER, vs_source);
    unsigned int fragment = compile(GL_FRAGMENT_SHADER, fs_source);

    prog_id = glCreateProgram();
    glAttachShader(prog_id, vertex);
    glAttachShader(prog_id, fragment);
    glLinkProgram(prog_id);

    int success;
    char info_log[512];
    glGetProgramiv(prog_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(prog_id, 512, NULL, info_log);
        throw std::exception(info_log);
    }

    // linked shaders no longer needed
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

unsigned int Shader::get_prog_id() const
{
    return prog_id;
}

void Shader::use() const
{
    glUseProgram(prog_id);
}

void Shader::set_vertex_attribute(char* name, GLenum type, int stride, void* pointer) const
{
    GLuint loc = glGetAttribLocation(prog_id, name);
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, type, GL_FALSE, stride, pointer);
}

void Shader::set_uniform_float(const char* name, float value) const
{
    glUniform1f(glGetUniformLocation(prog_id, name), value);
}

int Shader::compile(GLenum shader_type, const char** shader_source)
{
    unsigned int shader;
    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, shader_source, NULL);
    glCompileShader(shader);

    int  success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        throw std::exception(info_log);
    }

    return shader;
}