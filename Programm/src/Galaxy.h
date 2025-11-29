#ifndef GALAXY
#define GALAXY

#include "shader.h"
#define G 6.674*pow(10, -11) //m^3 kg^-1 switch 

class Star{
public:
    Vector2 speed;
    Vector2 position;

    double mass;
    double radius;
    const double max_speed = 10;

    Star();
    Star(double x, double y, double radius);

    void operator=(Star s);

    Vector2 force_field(Vector2 uTov);
    void apply_force(Vector2 f, double dt);
    void update_position(double dt);
};
enum Algorithm {
    Naive,
    Barnes_Hut,
    GPU
};

class Galaxy{
public:
    Galaxy(Algorithm type);
    ~Galaxy();

    void Update();
    void Draw();

private:
    // Objects varables
    double time_step = 0.01;
    int star_count;
    Star* stars;

    void calculate_force(int star_id);
    void Naive(int star_id);
    //Rendering variables

    const double star_radius = 10;
    unsigned int VAO;
    unsigned int VBO;
    Shader shader;
    Algorithm algo_type;
};
#endif
