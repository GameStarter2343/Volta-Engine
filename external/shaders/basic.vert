#version 460 core

layout(location = 0) in vec3 aPos;
uniform vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
uniform mat4 mvp;
out vec4 fragColor;

void main()
{
    if (color.a > 0.0) fragColor = color;
    else fragColor = vec4(aPos, 1.0);
    gl_Position = vec4(aPos, 1.0) * mvp;
}
