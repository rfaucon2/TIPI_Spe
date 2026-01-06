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

Galaxy::Galaxy(Algorithm type){
    this->algo_type = type;
    // Init list of stars
    this->star_count = 1000;
    this->stars = new Star[this->star_count];

    // Randomly generates stars
    std::random_device rd{}; 
    std::mt19937 gen{rd()};
    std::normal_distribution<float> d{0, 0.7};
    for(int i = 0; i < this-> star_count; i++)
    {
        double r = d(gen);
        double t = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3));
        this->stars[i] = Star(r*cos(t), r*sin(t), pow(10, 11));
    }
    std::cout << "generated\n"; 
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
        this->insert_star_in_tree(this->stars[i].x, this->stard[i].y, i);
}

void Galaxy::insert_star_in_tree(double x, double y, int id){

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
