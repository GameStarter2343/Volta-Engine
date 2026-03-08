#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uiTexture;
uniform vec4 tintColor = vec4(1.0, 1.0, 1.0, 1.0); // Default white

void main() {
    vec4 texColor = texture(uiTexture, TexCoord);
    FragColor = texColor * tintColor;
    if (FragColor.a < 0.01) discard; // Optional: discard fully transparent
}
