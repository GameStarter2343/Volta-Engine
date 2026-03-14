#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos = vec3(2.0, 2.0, 2.0);
uniform vec3 viewPos = vec3(0.0, 0.0, 3.0);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 color = vec3(1.0, 0.8, 0.6);
    vec3 result = color * diff + color * 0.2;
    FragColor = vec4(result, 1.0);
}
