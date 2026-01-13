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
    double d = dist(uTov, Vector2(0, 0));
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
    this->star_count = 1000;
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
        std::cout <<r*cos(t) << "\t" << r*sin(t) << std::endl;
    }
    this->stars[0] = Star(0, 0, pow(10, 11));
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
}

void Galaxy::calculate_force(int star_id)
{
    switch (this->algo_type) {
        case Algorithm::Naive: 
            this->Naive(star_id);break;
        case Algorithm::Barnes_Hut: 
            break;
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

void Galaxy::Update(){
    if(this->algo_type == Algorithm::Barnes_Hut)
        this->Update_tree();

    for(int i = 0; i < this->star_count; i++){
        this->calculate_force(i);
        this->stars[i].update_position(this->time_step);
    }
}
 void Galaxy::Update_tree(){
    for(int i = 0; i < this->star_count; i++)
        this->insert_star_in_tree(this->stars[i].position.x, this->stars[i].position.y, i);
}

void Galaxy::insert_star_in_tree(double x, double y, int id, QuadTree *tree){
    if(*tree.star_id = -1) { // if not a leaf
        int quadrant = (x >= *tree.center.x) + 2*(y < *tree.center.y);
        if((*tree)[quadrant] != std::null_ptr)
            insert_star_in_tree(x, y, id, (*tree)[quadrant]);
        else
            (*tree)[quadrant] = new Quadtree({std::null_ptr, std::null_ptr, std::null_ptr, std::null_ptr},
                                              id,
                                              (*tree).star_pos,
                                              (*tree).center + Vector2(this->window_size / pow(2, -(*tree)depth + 1)),
                                              (*tree).depth + 1;
    }
    else {
        int last_id = (*tree).star_id;
        Vector2 last_pos = (*tree).star_pos;
        (*tree).star_id = -1;
        insert_star_in_tree(x, y, id, tree);
        insert_star_in_tree(last_pos.x, last_pos.y, last_id, tree);
    }
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
