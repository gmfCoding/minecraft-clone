#version 330 core

in vec2 TexCoords;

uniform sampler2D texSampler;
const vec2 fxaaQualityRcpFrame = vec2(1.0 / 800, 1.0 / 600);

out vec4 FragColor;

#define FXAA_REDUCE_MIN   (1.0/128.0)
#define FXAA_REDUCE_MUL   (1.0/8.0)
#define FXAA_SPAN_MAX     8.0


// NOT WORKING
vec4 applyFxaa(sampler2D tex, vec2 pos, vec2 rcpFrame)
{
    
    vec3 rgbNW = texture(tex, pos + vec2(-1.0, -1.0) * rcpFrame).xyz;
    vec3 rgbNE = texture(tex, pos + vec2(1.0, -1.0) * rcpFrame).xyz;
    vec3 rgbSW = texture(tex, pos + vec2(-1.0, 1.0) * rcpFrame).xyz;
    vec3 rgbSE = texture(tex, pos + vec2(1.0, 1.0) * rcpFrame).xyz;
    vec3 rgbM = texture(tex, pos).xyz;
    float alphaM = texture(tex, pos).a;
    vec4 rgbaM  = vec4(
        (1.0/6.0) * (rgbNW + rgbNE + rgbSW + rgbSE + 2.0 * rgbM),
        alphaM);

    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbaM.xyz,  luma);
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) *
                          (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);

    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
              max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
              dir * rcpDirMin)) * rcpFrame;

    vec3 rgbA = 0.5 * (
        texture(tex, pos.xy + dir * (1.0 / 3.0 - 0.5)).xyz +
        texture(tex, pos.xy + dir * (2.0 / 3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * 0.5 + 0.25 * (
        texture(tex, pos.xy + dir * -0.5).xyz +
        texture(tex, pos.xy + dir * 0.5).xyz);

    float lumaB = dot(rgbB, luma);
    if ((lumaB < lumaMin) || (lumaB > lumaMax))
        FragColor = vec4(rgbA, 1.0);
    else
        FragColor = vec4(rgbB, 1.0);

    return FragColor;
}

void main()
{    
    FragColor = applyFxaa(texSampler, TexCoords, fxaaQualityRcpFrame);
}