#include <vector>
#include <string>
#include <iostream>


#include <glm/vec4.hpp>

#include "engine.hpp"
#include "Material.hpp"
#include "fileio.hpp"

class PostprocessEffect
{
    public:
    std::string name;
    Material* material;
    bool initialised = false;
    bool enabled = false;

    PostprocessEffect(const std::string& pName) : name(pName)
    {

    }

    PostprocessEffect(const std::string& pName, Material* pMaterial) : name(pName), material(pMaterial)
    {
        initialised = true;
    }

public:
    virtual void Setup()
    {
        if(material != nullptr)
            initialised = true;
    }

    virtual void Update()
    {
        glUseProgram(material->programID);
    }
};


/// @brief 
/// The game draws mainly to the prefx_fbo then the post processing is applied to the default fbo
class PostProcessing {

    public:
    bool effectsEnabled;


    public:
    std::vector<PostprocessEffect*> effects;

    PostprocessEffect* noeffect;

    // Full-screen sized quad vertex array object.
    GLuint postfx_vao;
    GLuint prefx_fbo, prefx_rbo, prefx_renderTexture;

    bool enable;

    PostProcessing() : effects()
    {
        noeffect = new PostprocessEffect("no_effect", new VertexFragmentCombinationMaterial("basic", getAssetPath({"shaders", "vertex_uv.shader"}),   getAssetPath({"shaders", "postfx", "noeffect.shader"})));
    }



    public:
    void Init()
    {
        const int &width = currentEngine->width;
        const int &height = currentEngine->height;

        // Set up vertex data (and buffer(s)) and attribute pointers
        static GLfloat vertices[] = {
            // Positions       // Texture Coords
            -1.0f,  1.0f, 0.2f,  0.0f, 1.0f,  // Top-left
            1.0f,  1.0f, 0.2f,  1.0f, 1.0f,  // Top-right
            1.0f, -1.0f, 0.2f,  1.0f, 0.0f,  // Bottom-right
            -1.0f, -1.0f, 0.2f,  0.0f, 0.0f   // Bottom-left
        };
        static GLuint indices[] = {
            0, 3, 1, // First triangle
            1, 3, 2  // Second triangle
        };
        
        GLuint postfx_vbo, postfx_ebo;
        glGenVertexArrays(1, &postfx_vao);
        glGenBuffers(1, &postfx_vbo);
        glGenBuffers(1, &postfx_ebo);

        glBindVertexArray(postfx_vao);

        glBindBuffer(GL_ARRAY_BUFFER, postfx_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, postfx_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // TexCoord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);


        // Set up the framebuffer object that the scene will draw too.
        glGenFramebuffers(1, &prefx_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, prefx_fbo);

        // Create a render buffer object to store the depth buffer
        glGenRenderbuffers(1, &prefx_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, prefx_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, prefx_rbo);

        glGenTextures(1, &prefx_renderTexture);
        glBindTexture(GL_TEXTURE_2D, prefx_renderTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, prefx_renderTexture, 0);

        // Check if the framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Begin()
    {
        effectsEnabled = ActiveEffectsCount() > 0 && enable;
        
        // Render the scene directly to the default buffer
        if(!effectsEnabled)
        {
            End();
        }
        else
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prefx_fbo);

        glEnable(GL_DEPTH_TEST);
        glm::vec4 cc = *currentEngine->clear_color;
        glClearColor(cc.x * cc.w, cc.y * cc.w, cc.z * cc.w, cc.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void End()
    {
        glDisable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }

    void Add(PostprocessEffect* effect, int order)
    {
        effects.insert(effects.begin() + order, effect);
        effect->Setup();
    }

    void ApplyEffects()
    {
        if(!effectsEnabled)
            return;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, prefx_renderTexture);
        
        for (auto i : effects)
        {
            if(i->initialised && i->enabled)
                i->Update();
            DrawScreenSizedQuad();
        }
    }

    void DrawScreenSizedQuad()
    {
        if(!effectsEnabled)
            return;

        glBindVertexArray(postfx_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }


    int ActiveEffectsCount()
    {
        int active = 0;
        for (auto i : effects)
        {
            if(i->enabled)
                active++;
        }

        return active;
    }
};