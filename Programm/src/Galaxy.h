#ifndef GALAXY
#define GALAXY

#include "shader.h"


class Galaxy{
public:
    Galaxy();
    ~Galaxy();

    void Update();
    void Draw();

private:
    // Objects varables
    int star_count;
    Vector2* stars; 

    //Rendering variables
    const double star_radius = 10;
    unsigned int VAO;
    unsigned int VBO;
    Shader shader;

    // Simulation parameters
    const double star_weight = 10;
};
#endif
