#version 450 core

flat in vec3 fragmentColor;
in float isPoint;

out vec4 FragColor;

void main() {
    FragColor = vec4(fragmentColor, 1.0);
}
