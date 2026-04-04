#version 450 core
out vec4 FragColor;
in vec4 fragColor;
void main()
{
    vec2 p = gl_PointCoord * 2.0 - 1.0;
    float dist = length(p);

    if (dist > 1.0) discard;

    float alpha = 1.0 - dist;
    alpha = pow(alpha, 1.5);

    FragColor = fragColor * vec4(1.0f, 1.0f, 1.0f, alpha);
}
