#ifndef GALAXY
#define GALAXY

class Galaxy{
public:
    Galaxy();
    ~Galaxy();

    void Update();
    void Draw();

private:
    // Objects varables
    const int star_count = 100;
    int* stars;

    // Simulation parameters
    const double star_weight = 10;
};
#endif
