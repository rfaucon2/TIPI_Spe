#ifndef GALAXY
#define GALAXY

#include <queue>
#include "shader.h"

#define G                   6.674*pow(10, -11)      //[m^3 s^-2 kg^-1]
#define PI                  3.14159
#define EPSILON             0.001
#define AL                  * 9.5*pow(10,15)        // Année lumière [m]
#define M_SOL               * 2*pow(10, 30)         // Masse solaire [kg]
#define K                   (G M_SOL / (1 AL* 1 AL * 1 AL))
#define SCREEN_WORLD_WIDTH  200000                  // [AL]

Vector2 world_to_screen_coord(Vector2 pos);

class QuadTree{
    QuadTree *childs[4]; 
        //      ^
        //   0  │  1
        // ─────┼────>
        //   2  │  3
        //      │
    std::vector<int> star_list;
    Vector2 mass_center; // [AL]
    Vector2 quad_center; // [AL]
    double mass;         // [M_SOL]
    int depth;

    int get_quadrant(Vector2 pos);
};

class Star{
    Vector2 speed;      // [m s^-1]
    Vector2 position;   // [Al]
    double mass;        // [M_SOL]
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
    Galaxy(int window_size, Algorithm type, unsigned long star_count, double theta, unsigned long speed);
    ~Galaxy();

    void Update();
    void Draw();
    Vector2 get_star_pos(int id);
    Vector2 get_star_speed(int id);
    double get_precision();
private:
    // Objects varables
    double time_step = pow(10, 15);
    unsigned long star_count;
    Star *stars;

    // Barnes-Hut 
    double BH_theta;
    QuadTree *BH_tree;
    std::queue<QuadTree*> BH_queue;

    // Force functions
    void calculate_force(int star_id);
    void Naive(int star_id);
    void Barnes_hut(int star_id);
   
    // Tree functions
    bool is_on_screen(int id);
    void insert_star_in_tree (double x, double y, int id, QuadTree *tree);
    void Update_tree();
    void Delete_tree(QuadTree* tree);
    

    //Rendering variables
    int window_size;
    unsigned int VAO;
    unsigned int VBO;
    Shader shader;
    Algorithm algo_type;
};
#endif
