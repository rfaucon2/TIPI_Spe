#ifndef GALAXY
#define GALAXY

#include "shader.h"
#define G 6.674*pow(10, -11) //m^3 kg^-1 switch 
#define PI 3.14159

class QuadTree{
public:
    QuadTree *childs[4]; 
        //      ^
        //   0  │  1
        // ─────┼────>
        //   2  │  3
        //      │
    int star_id;
    Vector2 star_pos;
    Vector2 center;
    int depth;
};

class Star{
public:
    Vector2 speed;
    Vector2 position;

    double mass;
    double radius;
    const double max_speed = 10;

    Star();
    Star(double x, double y, double mass);

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
    Galaxy(int window_size, Algorithm type);
    ~Galaxy();

    void insert_star_in_tree (double x, double y, int id, QuadTree *tree);
    void Update();
    void Update_tree();
    void Draw();

private:
    // Objects varables
    double time_step = 0.00001;
    int star_count;
    Star* stars;

    void calculate_force(int star_id);
    void Naive(int star_id);
    
    //Rendering variables
    int window_size;
    const double star_radius = 10;
    unsigned int VAO;
    unsigned int VBO;
    Shader shader;
    Algorithm algo_type;
};
#endif
