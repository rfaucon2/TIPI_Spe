#ifndef GALAXY
#define GALAXY

#include <math>
#include "shader.h"

class Vector2{
public:
    double x, y;
    Vector2(double x, double y);
}

class Galaxy{
public:
    Galaxy();
    ~Galaxy();

    void Update();
    void Draw();

private:
    // Objects varables
    const int star_count = 1;
    Vector2* stars; 

    //Rendering variables
    const double star_radius = 10;
    unsigned int star_VAO;
    unsigned int star_VBO;
    Shader shader;

    // Simulation parameters
    const double star_weight = 10;
};
#endif
