#version 330

uniform sampler2D tex; // texture uniform
uniform bool dspUV;
in vec2 ftexcoord;

layout(location = 0) out vec4 FragColor;

void main() {
   if(dspUV)
   {
      FragColor = vec4(ftexcoord.x, ftexcoord.y, 0.0, 0.0);
   }else
   {
      FragColor = texture(tex, ftexcoord);
   }
}