class Camera;

class PlayerController {
    public:
    Camera* camera;

    float height = 1.6f;
    glm::vec3* eyeposition;

    void Control(PlayerMove move);
    void OnMouseInput(void* _input);

    void ProcessMovement();
};
