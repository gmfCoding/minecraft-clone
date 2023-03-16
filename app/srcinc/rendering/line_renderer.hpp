#include <vector>
#include <glad.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "physics/transform.hpp"
#include "Material.hpp"
#include "Mineclone.hpp"
#include "Camera.hpp"

class LineRenderer
{
    public:
    Material material;
    MaterialProperties properties;

    Transform* transform;

    std::vector<glm::vec3> lines;
    bool dynamic;
    bool dirty;

    GLuint VAO, VBO;

    LineRenderer(Material pMaterial, bool pDynamic = false);

    void Bind();
    void Upload();
    void Render();
};


