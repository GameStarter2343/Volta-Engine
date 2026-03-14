#version 450 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 mvp;
uniform float minY;
uniform float maxY;
uniform vec3 color1;
uniform vec3 color2;

// Коэффициент силы затемнения (настрой под свой масштаб)
uniform float areaDarkeningScale = 2;

out vec3 fragmentColor;

void main() {
    float centerY = (gl_in[0].gl_Position.y + gl_in[1].gl_Position.y + gl_in[2].gl_Position.y) / 3.0;
    float t = clamp((centerY - maxY) / (minY - maxY), 0.0, 1.0);
    vec3 finalColor = mix(color1, color2, t);

    for (int i = 0; i < 3; i++) {
        gl_Position = mvp * gl_in[i].gl_Position;
        fragmentColor = finalColor;
        EmitVertex();
    }
    EndPrimitive();
}
