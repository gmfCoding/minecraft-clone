#include "PlayerMove.hpp"


class Camera;
class World;
class PlayerController;
class PostProcessing;
class PostprocessEffect;

class Mineclone
{
    public:
    static Mineclone* instance;

    PlayerMove playerMove{};
    Camera* camera;
    World* world;
    World* worldtest;
    PlayerController* playerController;

    PostProcessing* postprocess;
    PostprocessEffect* underwater_effect;

    int* width;
    int* height;

    Mineclone();

    void Start();
    void SetupPostProcessing();

    void Update();  

    void RenderingBegin();
    void RenderingEnd();
};