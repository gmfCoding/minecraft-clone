class Camera;

class PlayerController {
    public:
    Camera* camera;

    void Control(PlayerMove move);
    void OnMouseInput(void* _input);
};
