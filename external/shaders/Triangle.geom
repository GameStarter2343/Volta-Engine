#version 450 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 mvp;
uniform float minY;
uniform float midY;
uniform float maxY;
uniform vec3 color1;
uniform vec3 color2;
uniform vec3 color3;

uniform float areaDarkeningScale = 2.0;

out vec3 fragmentColor;

void main() {
    float centerY = (gl_in[0].gl_Position.y + gl_in[1].gl_Position.y + gl_in[2].gl_Position.y) / 3.0;

    float denom = (minY - maxY);
    float t = (denom != 0.0) ? clamp((centerY - maxY) / denom, 0.0, 1.0) : 0.0;

    float midT = (denom != 0.0) ? clamp((midY - maxY) / denom, 0.0, 1.0) : 0.5;

    if (midT <= 0.0 || midT >= 1.0) {
        midT = 0.5;
    }

    vec3 finalColor;
    if (t <= midT) {
        float local = (midT > 0.0) ? (t / midT) : 0.0;
        finalColor = mix(color1, color2, local);
    } else {
        float local = (t - midT) / (1.0 - midT);
        finalColor = mix(color2, color3, local);
    }

    for (int i = 0; i < 3; ++i) {
        gl_Position = mvp * gl_in[i].gl_Position;
        fragmentColor = finalColor;
        EmitVertex();
    }
    EndPrimitive();
}
