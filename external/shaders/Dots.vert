#version 450 core
layout(location = 0) in vec2 inPos;
float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

// Функция сглаженного шума
float smoothNoise(vec2 uv) {
    vec2 i = floor(uv); // Целая часть (индекс клетки)
    vec2 f = fract(uv); // Дробная часть (положение внутри клетки)

    // Вычисляем значения в 4-х углах квадрата
    float a = rand(i);
    float b = rand(i + vec2(1.0, 0.0));
    float c = rand(i + vec2(0.0, 1.0));
    float d = rand(i + vec2(1.0, 1.0));

    // Плавная кривая интерполяции (аналог smoothstep)
    // Она убирает резкие переходы на границах клеток
    vec2 u = f * f * (3.0 - 2.0 * f);

    // Смешиваем значения углов
    return mix(a, b, u.x) +
        (c - a) * u.y * (1.0 - u.x) +
        (d - b) * u.x * u.y;
}
uniform mat4 mvp;
uniform float pointSizeMin;
uniform float pointSizeMax;

void main()
{
    gl_Position = mvp * vec4(inPos, 0.0, 1.0);
    gl_PointSize = mix(pointSizeMin, pointSizeMax, smoothNoise(inPos.xy * 40));
}
