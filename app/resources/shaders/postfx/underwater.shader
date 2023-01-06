#version 330 core

in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D texture;
uniform float hueShift;

const vec3 L = vec3(0.2125, 0.7154, 0.0721);

void main()
{
    vec4 color = texture2D(texture, TexCoord);
    fragColor = (color * vec4(.2, .2, 1.1, 1));
}