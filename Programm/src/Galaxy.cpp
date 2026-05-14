#include "Galaxy.h"

Star::Star(){}
Star::Star(double x, double y, double mass){
    this->position = Vector2(x, y);
    this->speed = Vector2(0, 0);
    this->mass = mass;
    this->radius = mass;
}
void Star::operator=(Star s){
    this->speed = s.speed;
    this->position = s.position;
    this->mass = s.mass;
    this->radius = s.radius;

}
Vector2 Star::force_field(Vector2 uTov){
    double d = std::max(dist(uTov, Vector2(0, 0)), EPSILON);
    return -(G*this->mass/pow(d, 3)) * uTov;  
}
void Star::apply_force(Vector2 f, double dt){
    this->speed += dt * f; 
}
void Star::update_position(double dt){
    this->position += dt * this->speed;
}

Galaxy::Galaxy(int window_size, Algorithm type){
    this->algo_type = type;
    this->window_size = window_size;
    // Init list of stars
    this->star_count = 10;
    this->stars = new Star[this->star_count];

    // Randomly generates stars
    std::random_device rd{}; 
    std::mt19937 gen{rd()};
    std::normal_distribution<float> d{0, 0.7};
    for(int i = 0; i < this->star_count; i++)
    {
        double r = d(gen);
        double t = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3));
        this->stars[i] = Star(r*cos(t), r*sin(t), pow(10, 11));
    }
    this->stars[0] = Star(0, 0, pow(10, 19));
    
    
    this->BH_tree = new QuadTree{{nullptr, nullptr, nullptr, nullptr}, -1, Vector2(0,0), Vector2(window_size/2, window_size/2), 0};

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
    for(int i = 0; i < this->star_count; i++)
    {
        this->shader.use();
        shader.set_vec2("offset", this->stars[i].position);
        glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void Galaxy::calculate_force(int star_id)
{
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
 
void Galaxy::Barnes_hut(int star_id){
    TODO: CHANGE TO VECTOR
    this->BH_queue = std::queue<QuadTree*>();
    for(int i = 0; i < 4; i++)
        if (this->BH_tree->childs[i] != nullptr)
            BH_queue.push(this->BH_tree->childs[i]);

    for(;!BH_queue.empty();BH_queue.pop())
    {
        QuadTree* current = BH_queue.front();
        if(current->star_id != -1) // if node is a leaf
        {
            Vector2 F = this->stars[current->star_id].force_field(this->stars[star_id].position - this->stars[current -> star_id].position);
            this->stars[star_id].apply_force(F, this->time_step);
    
        }
        else // If its not a leaf
        {
            // Apply Barnes-Hut test
            double d = dist(this->stars[star_id].position, current->quad_center);
            double s = this->window_size* pow(2, -current->depth);
            
            if (s/d < this->BH_theta) // If sufficiently far away
            {
                Vector2 F = Star(current->star_pos.x, current->star_pos.y, current->mass).force_field(this->stars[star_id].position - current->star_pos);
                this->stars[star_id].apply_force(F, this->time_step);
            }
            else // If too close
            {
                for(int i = 0; i < 4; i++)
                    if (current->childs[i] != nullptr)
                        BH_queue.push(current->childs[i]);
            }
        }
    }
}

bool Galaxy::is_on_screen(int id){
    Vector2 pos = this->stars[id].position;
    return (pos.x <= 0.5)&&(pos.x >=-0.5)&&(pos.y>=-0.5)&&(pos.y<=0.5); 

}

void Galaxy::insert_star_in_tree(double x, double y, int id, QuadTree *tree){
    if(!this->is_on_screen(id))
        return;
    std::cout << id << "\t " << x << ", " << y << "\t" << tree->depth + 1 << "\n";
    if(tree->star_id == -1) { // if not a leaf
        TODO: ADDAPT TO VECTOR 
        int quadrant = (x >= tree->quad_center.x) + 2*(y < tree->quad_center.y);
        if(tree->childs[quadrant] != nullptr) // if quadrant not empty, modify mass stuff and recurse into it
        {
            tree->star_pos = (tree->mass*tree->star_pos + this->stars[id].mass*this->stars[id].position)/(tree->mass + this->stars[id].mass);
            tree->mass += this->stars[id].mass;
            insert_star_in_tree(x, y, id, tree->childs[quadrant]);
        }
        else{ // if quadrant empty, then displace the leaf
            tree->childs[quadrant] = new QuadTree {
                                              {nullptr, nullptr, nullptr, nullptr},
                                              id,
                                              tree->star_pos,
                                              tree->quad_center + Vector2(pow(-1, 1 - (quadrant % 2)) / pow(2, tree->depth + 1),
                                                                     pow(-1,     (quadrant / 2)) / pow(2, tree->depth + 1)),
                                              tree->mass,
                                              tree->depth + 1
            };
        }
    }
    else {
        TODO: DISTANCE CHECK and vector handling
        if(tree->star_list.size() > 1 || dist(tree->star_list[0], this->stars[id].position) < EPSILON) {// if already multiple stars in the cell or too close to a star
            tree->mass_center = (tree->mass * tree->mass_center + this->stars[id].mass * this->stars[id].position)/(tree->mass + this_>stars[id].mass);
            tree->mass += this->stars[id].mass;
            tree->star_list.append(id);
        }
        int last_id = tree->star_id;
        Vector2 last_pos = tree->star_pos;
        tree->star_id = -1;
        tree->star_pos = (tree->mass*tree->star_pos + this->stars[id].mass*this->stars[id].position)/(tree->mass + this->stars[id].mass);
        tree->mass += this->stars[id].mass;
        insert_star_in_tree(x, y, id, tree);
        insert_star_in_tree(last_pos.x, last_pos.y, last_id, tree);
    }
}

void Galaxy::Update_tree(){
    this->BH_tree = new QuadTree{{nullptr, nullptr, nullptr, nullptr}, , Vector2(0, 0), 0, 0};
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



