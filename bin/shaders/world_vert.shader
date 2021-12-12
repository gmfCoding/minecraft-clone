
#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
  
out vec4 vertexColor; // specify a color output to the fragment shader

uniform mat4 transform;
uniform vec4 col_uni;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    //gl_Position = proj * view * model * vec4(aPos, 1.0);
    vertexColor = vec4(col_uni);//vec4(1.0, 0.1, 0.3, 1.0);
}