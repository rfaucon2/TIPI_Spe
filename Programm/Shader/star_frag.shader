#version 330 core
in vec2 vPos;
out vec4 FragColor;

void main()
{
    float uAspect = 1000.0/1000.0;
    vec2 pos = vPos;
    pos.x *= uAspect;

    float radius = 0.01;
    if (length(pos) > radius)
        discard;   // Make outside transparent (or background)
    FragColor = (1.0 - smoothstep(0, radius, length(pos))) * vec4(132.0/255.0, 130.0/255.0, 191.0/255.0, 1.0);
}
