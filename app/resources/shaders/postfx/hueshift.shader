#version 330 core

in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D texture;
uniform float hueShift;

const vec3 L = vec3(0.2125, 0.7154, 0.0721);

void main()
{
    vec4 color = texture2D(texture, TexCoord);
    float luminance = dot(color.rgb, L);
    vec3 chroma = color.rgb - luminance;
    mat3 rotation = mat3(1.0, 0.0, 0.0,
                         0.0, cos(hueShift), -sin(hueShift),
                         0.0, sin(hueShift), cos(hueShift));
    fragColor = vec4(luminance + rotation * chroma, color.a);
}