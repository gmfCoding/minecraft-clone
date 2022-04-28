#include "LoadGlad.h"
#include <string>
#include <vector>
#include <glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "gmfc_image.hpp"
#include "TextureManager.hpp"
#include "MaterialSystem.hpp"

class ImageExample
{
private:
     // get texture uniform location
    GLuint shader_program;
    // vao and vbo handle
    GLuint vao, vbo, ibo;

    GLuint texture;

    GLint uniTexture;
    GLuint uniTransform;

public:
    ImageExample();
    void Init(const GLuint gpuTexture);
    void Update();
    ~ImageExample();
};

ImageExample::ImageExample()
{
}

bool check_shader_compile_status(GLuint obj) {
    GLint status;
    glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        GLint length;
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetShaderInfoLog(obj, length, &length, &log[0]);
        std::cerr << &log[0];
        return false;
    }
    return true;
}

void ImageExample::Init(const GLuint gpuTexture)
{
    shader_program = MaterialSystem::materialMap["alt_textured"]->programID;
 
    // generate and bind the vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // generate and bind the vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
            
    // data for a fullscreen quad (this time with texture coords)
    GLfloat vertexData[] = {
    //  X     Y     Z           U     V     
       1.0f, 1.0f, 0.0f,       1.0f, 1.0f, // vertex 0
      -1.0f, 1.0f, 0.0f,       0.0f, 1.0f, // vertex 1
       1.0f,-1.0f, 0.0f,       1.0f, 0.0f, // vertex 2
      -1.0f,-1.0f, 0.0f,       0.0f, 0.0f, // vertex 3
    }; // 4 vertices with 5 components (floats) each

    // fill with data
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*5, vertexData, GL_STATIC_DRAW);
                    
    // set up generic attrib pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (char*)0 + 0*sizeof(GLfloat));
 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (char*)0 + 3*sizeof(GLfloat));
    
    
    // generate and bind the index buffer object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            
    GLuint indexData[] = {
        0,1,2, // first triangle
        2,1,3, // second triangle
    };

    // fill with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*2*3, indexData, GL_STATIC_DRAW);
    
    // "unbind" vao
    glBindVertexArray(0);

    texture = gpuTexture;

    // bind the texture
    glBindTexture(GL_TEXTURE_2D, texture);
    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set the filter to nearest
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void ImageExample::Update()
{
     // use the shader program
        glUseProgram(shader_program);

        // bind texture to texture unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        

        GLCall(uniTexture = glGetUniformLocation(shader_program, "tex"));
        GLCall(uniTransform = glGetUniformLocation(shader_program, "MVP"));

        // set texture uniform
        glUniform1i(uniTexture, 0);

        glm::mat4 mvp = Renderer::camera->projection * Renderer::camera->view;
        glUniformMatrix4fv(uniTransform, 1, GL_FALSE,  glm::value_ptr(mvp));
        
        // bind the vao
        glBindVertexArray(vao);
        
        // draw
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
}

ImageExample::~ImageExample()
{
}

    //     // shader source code
    // std::string vertex_source =
    //     "#version 330\n"
    //     "layout(location = 0) in vec4 vposition;\n"
    //     "layout(location = 1) in vec2 vtexcoord;\n"
    //     "uniform mat4 MVP;\n"
    //     "out vec2 ftexcoord;\n"
    //     "void main() {\n"
    //     "   ftexcoord = vtexcoord;\n"
    //     "   gl_Position = MVP * vposition;\n"
    //     "}\n";
        
    // std::string fragment_source =
    //     "#version 330\n"
    //     "uniform sampler2D tex;\n" // texture uniform
    //     "in vec2 ftexcoord;\n"
    //     "layout(location = 0) out vec4 FragColor;\n"
    //     "void main() {\n"
    //     "   FragColor = texture(tex, ftexcoord);\n"
    //     "}\n";
   
    // // program and shader handles
    // GLuint vertex_shader, fragment_shader;
    
    // // we need these to properly pass the strings
    // const char *source;
    // int length;

    // // create and compiler vertex shader
    // vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    // source = vertex_source.c_str();
    // length = vertex_source.size();
    // glShaderSource(vertex_shader, 1, &source, &length); 
    // glCompileShader(vertex_shader);

 
    // // create and compiler fragment shader
    // fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    // source = fragment_source.c_str();
    // length = fragment_source.size();
    // glShaderSource(fragment_shader, 1, &source, &length);   
    // glCompileShader(fragment_shader);

        
    // // create program
    // shader_program = glCreateProgram();
    
    // // attach shaders
    // glAttachShader(shader_program, vertex_shader);
    // glAttachShader(shader_program, fragment_shader);
    
    // // link the program and check for errors
    // glLinkProgram(shader_program);