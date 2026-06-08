#include "Galaxy.h"

Vector2 world_to_screen_coord(Vector2 pos){
    return Vector2(2*pos.x/SCREEN_WORLD_WIDTH, 2*pos.y/SCREEN_WORLD_WIDTH);
}

QuadTree::get_quadrant(Vector2 pos){
    return (pos.x >= this->quad_center.x) + 2*(pos.y < this->quad_center.y);
}

Star::Star(){}
Star::Star(double x, double y, double mass){
    this->position = Vector2(x, y);
    this->speed = Vector2(0, 0);
    this->mass = mass;
}

void Star::operator=(Star s){
    this->speed = s.speed;
    this->position = s.position;
    this->mass = s.mass;
}

Vector2 Star::force_field(Vector2 uTov){
    double d = std::max(dist(uTov, Vector2(0, 0)), EPSILON);
    return -(K*this->mass/pow(d, 3)) * uTov;  
}

void Star::apply_force(Vector2 f, double dt){
    double d = dist(dt*f, Vector2(0, 0));
    if (d < max_speed)
        this->speed += dt * f;
    else 
        this->speed += dt*this->max_speed*(1/d)*f;
}

void Star::update_position(double dt){
    this->position += dt * this->speed;
}

Galaxy::Galaxy(int window_size, Algorithm type, unsigned long star_count, double theta, unsigned long seed){
    this->algo_type = type;
    this->window_size = window_size;
    // Init list of stars
    this->star_count = star_count;
    this->stars = new Star[this->star_count];
    this->BH_theta = theta;

    // Randomly generates stars
    //std::random_device rd{}; 
    std::mt19937 gen(seed);
    std::normal_distribution<float> d{0, 0.7};
    for(int i = 0; i < this->star_count; i++)
    {
        double r = d(gen) * SCREEN_WORLD_WIDTH/2;
        double t = (PI/2) * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3));
        this->stars[i] = Star(r*cos(t), r*sin(t), 1);
    }
    this->stars[0] = Star(0, 0, 4.4 * pow(10, 6));

    // Init shader
    this->shader = Shader("Shader/star_vert.shader", "Shader/star_frag.shader");
    //Create the star object for rendering
    float vertices[] = {// Array holdind the vertices of the square holding the circle
        // First triangle 
         0.5, 0.5,
         0.5,-0.5,
        -0.5, 0.5,
        // Second triangle 
         0.5,-0.5,
        -0.5,-0.5l,
        -0.5, 0.5
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO); // Bind the VAO first

    // Then configure the VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);
    
    // Finaly configure the VAO
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
    glBindVertexArray(0); // Unbind the VAO
}

Galaxy::~Galaxy(){
    delete this->stars;
    delete this->BH_tree;
}

void Galaxy::Update(){
    if(this->algo_type == Algorithm::Barnes_Hut){ 
        this->Update_tree();
    }
    for(int i = 0; i < this->star_count; i++){
        this->calculate_force(i);
        this->stars[i].update_position(this->time_step);
    }
    if(this->algo_type == Algorithm::Barnes_Hut)
        Delete_tree(this->BH_tree);
}

void Galaxy::Draw(){
    for(int i = this->star_count -1; i >= 0; i--)
    {
        this->shader.use();
        shader.set_vec2("offset", world_to_screen_coord(this->stars[i].position));
        if(this->stars[i].mass < pow(10, 6)) 
            shader.set_float("col", 0);
        else 
            shader.set_float("col", 1);
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

Vector2 Galaxy::get_star_pos(int id){
    return this->stars[id].position;
}

Vector2 Galaxy::get_star_speed(int id){
    return this->stars[id].speed;
}

double Galaxy::get_precision(){
    return this->BH_theta;
}

void Galaxy::calculate_force(int star_id){
    switch (this->algo_type) {
        case Algorithm::Naive: 
            this->Naive(star_id);break;
        case Algorithm::Barnes_Hut: 
            this->Barnes_hut(star_id);break;
        case Algorithm::GPU: 
            break;
    };
}

void Galaxy::Naive(int star_id){
    for(int i = 0; i < this->star_count; i++){
        if(i == star_id)
            i++;
        Vector2 F = this->stars[i].force_field(this->stars[star_id].position - this->stars[i].position);
        this->stars[star_id].apply_force(F, this->time_step);
    }
}

bool is_in_vector(std::vector<int> v, int val){
    for(int i = 0; i < v.size(); i++)
        if(v[i] == val)
            return true;
    return false;
}

void Galaxy::Barnes_hut(int star_id){
    this->BH_queue = std::queue<QuadTree*>();
    for(int i = 0; i < 4; i++)
        if (this->BH_tree->childs[i] != nullptr)
            BH_queue.push(this->BH_tree->childs[i]);

    for(;!BH_queue.empty();BH_queue.pop())
    {
        QuadTree* current = BH_queue.front();
        if(current->star_list.size() == 1){
            Vector2 F = this->stars[current->star_list[0]].force_field(this->stars[star_id].position - this->stars[current->star_list[0]].position);
            this->stars[star_id].apply_force(F, this->time_step);
        }
        // If the star is in the quadrant's star list, don't approximate
        else if (is_in_vector(current->star_list, star_id)){
            for(int i = 0; i < current->star_list.size(); i++){
                if(current->star_list[i] == star_id)
                    continue;
                Vector2 F = this->stars[current->star_list[i]].force_field(this->stars[star_id].position - this->stars[current->star_list[i]].position);
                this->stars[star_id].apply_force(F, this->time_step);
            }
        }
        // If more than one star are in the quadrant, approximate
        else if(current->star_list.size() > 1){
            Vector2 F = Star(current->mass_center.x, current->mass_center.y, current->mass).force_field(this->stars[star_id].position - current->mass_center);
            this->stars[star_id].apply_force(F, this->time_step);
        }
        // If its not a leaf
        else{ 
            // Apply Barnes-Hut test
            double d = dist(this->stars[star_id].position, current->mass_center);
            double s = SCREEN_WORLD_WIDTH*pow(2, -current->depth+1);
            
            if (s/d < this->BH_theta){ // If sufficiently far away
                Vector2 F = Star(current->mass_center.x, current->mass_center.y, current->mass).force_field(this->stars[star_id].position - current->mass_center);
                this->stars[star_id].apply_force(F, this->time_step);
            }
            else{ // If too close
                for(int i = 0; i < 4; i++)
                    if (current->childs[i] != nullptr)
                        BH_queue.push(current->childs[i]);
            }
        }
    }
}

bool Galaxy::is_on_screen(int id){
    Vector2 pos = this->stars[id].position;
    return (pos.x <= SCREEN_WORLD_WIDTH/2)&&(pos.x >=-SCREEN_WORLD_WIDTH/2)&&(pos.y>=-SCREEN_WORLD_WIDTH/2)&&(pos.y<=SCREEN_WORLD_WIDTH/2); 
}

void Galaxy::insert_star_in_tree(double x, double y, int id, QuadTree *tree){
    if(!this->is_on_screen(id))
        return;
    if(tree->star_list.size() == 0) { // if not a leaf
        int quadrant = tree->get_quadrant(Vector2(x, y));
        if(tree->childs[quadrant] != nullptr) // if quadrant not empty, modify mass stuff and recurse into it
        {
            tree->mass_center = (tree->mass * tree->mass_center + this->stars[id].mass*this->stars[id].position)/(tree->mass + this->stars[id].mass);
            tree->mass += this->stars[id].mass;
            insert_star_in_tree(x, y, id, tree->childs[quadrant]);
        }
        else{ // if quadrant empty, then create leaf
            tree->childs[quadrant] = new QuadTree {
                                                {nullptr, nullptr, nullptr, nullptr},
                                                std::vector<int>{id},
                                                Vector2(x, y),
                                                tree->quad_center + SCREEN_WORLD_WIDTH*(pow(2, -tree->depth - 1))*Vector2(pow(-1,1-(quadrant%2)),pow(-1,(quadrant/2))),
                                                tree->mass,
                                                tree->depth + 1
            };
        }
    }
    else {
        if(tree->star_list.size() > 1 || SCREEN_WORLD_WIDTH*pow(2, -tree->depth+1) < EPSILON) {// if already multiple stars in the cell or too close to a star
            tree->mass_center = (tree->mass * tree->mass_center + this->stars[id].mass * this->stars[id].position)/(tree->mass + this->stars[id].mass);
            tree->mass += this->stars[id].mass;
            tree->star_list.push_back(id);
        }
        else{
            int last_id = tree->star_list[0];
            Vector2 last_pos = this->stars[last_id].position;
            tree->star_list = std::vector<int>();
            tree->mass_center = (tree->mass*tree->mass_center + this->stars[id].mass*this->stars[id].position)/(tree->mass + this->stars[id].mass);
            tree->mass += this->stars[id].mass;
            insert_star_in_tree(x, y, id, tree);
            insert_star_in_tree(last_pos.x, last_pos.y, last_id, tree);
        }
       
    }
}

void Galaxy::Update_tree(){
    this->BH_tree = new QuadTree{{nullptr, nullptr, nullptr, nullptr}, std::vector<int>(), Vector2(0, 0), Vector2(0, 0), 0, 0};
    for(int i = 0; i < this->star_count; i++){
        if(!this->is_on_screen(i))
            continue;
        this->insert_star_in_tree(this->stars[i].position.x, this->stars[i].position.y, i, this->BH_tree);
    }
}

void Galaxy::Delete_tree(QuadTree* tree){
    for(int i = 0; i < 4; i++)
    {
        if(tree->childs[i] != nullptr)
            Delete_tree(tree->childs[i]);
    }
    delete tree;
}



