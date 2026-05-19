#version 330 core

layout(location = 0) in vec2 aPos;

uniform vec2 offset;
uniform float col;

out float c;
out vec2 vPos;

void main()
{
	gl_Position = vec4(aPos + offset, 0.0, 1.0);
    vPos = aPos;
    c = col;
}
