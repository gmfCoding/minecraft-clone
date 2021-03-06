
#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
  
out vec4 vertexColor; // specify a color output to the fragment shader

uniform mat4 transform;
uniform vec4 col_uni;
// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 proj;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    //gl_Position = proj * view * model * vec4(aPos, 1.0);
    vertexColor = vec4(col_uni);//vec4(1.0, 0.1, 0.3, 1.0);
}

// #version 330 core
// layout (location = 0) in vec3 pos; // the position variable has attribute position 0
  
// out vec4 vertexColor; // specify a color output to the fragment shader



// void main()
// {
//     gl_Position = proj * view * model * vec4(pos, 1.0); // see how we directly give a vec3 to vec4's constructor
//     vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
// }