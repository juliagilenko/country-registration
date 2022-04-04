R"(
#version 330 core

in vec3 colour3;
out vec4 colour4;

void main()
{
    colour4 = vec4(colour3, 1.0);
}
)"