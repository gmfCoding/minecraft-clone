#include "Rendering.hpp"
#include "MaterialSystem.hpp"
#include "BoxMeshGen.hpp"

class PrimitiveCube : public Object
{
private:
    /* data */
    static GLuint vao;
    Mesh mesh;
public:
    PrimitiveCube(/* args */);
    ~PrimitiveCube();

    void Render()
    {
        Renderer::RenderObject(this);
    }

};

PrimitiveCube::PrimitiveCube(/* args */)
{
    renderer = new MeshRenderer("basic");

    BoxMeshGen* bmg = new BoxMeshGen();
    mesh = bmg->generate();
    renderer->Bind(&mesh);

    delete bmg;
}

PrimitiveCube::~PrimitiveCube()
{
    delete renderer;
}
