R"(
#version 330 core

in vec3 position;
in vec3 colour;

uniform float s;
uniform float x;
uniform float y;

out vec3 colour3;

void main()
{
    mat4 scale = mat4(
    s, 0, 0, 0,
    0, s, 0, 0,
    0, 0, s, 0,
    0, 0, 0, 1);

    mat4 translation = mat4(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    x, y, 0, 1);

    gl_Position = translation * scale * vec4(position, 1.0);
    colour3 = colour;
}
)"