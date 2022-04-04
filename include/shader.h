#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glad/glad.h> 

/*
* A shader program.
*/
class Shader
{
public:
    Shader(const char** vs_source, const char** fs_source);
    unsigned int get_prog_id() const;
    void use() const;
    void set_uniform_float(const char* name, float value) const;
    void Shader::set_vertex_attribute(char* name, GLenum type, int stride, void* pointer) const;

private:
    unsigned int prog_id;
    int compile(GLenum shader_type, const char** shader_source);
};

#endif