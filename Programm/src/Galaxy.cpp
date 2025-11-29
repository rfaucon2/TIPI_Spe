#include "Galaxy.h"

Star::Star(){}
Star::Star(double x, double y, double radius){
    this->position = Vector2(x, y);
    this->speed = Vector2(0, 0);
    this->radius = radius;
}
void Star::operator=(Star s){
    this->speed = s.speed;
    this->position = s.position;

    this->mass = s.mass;
    this->radius = s.radius;

}
Vector2 Star::force_field(Vector2 uTov){
    double d = dist(uTov, Vector2(0, 0));
    return G*this->mass/pow(d, 3) * uTov;  
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
    this->star_count = 100;
    this->stars = new Star[this->star_count];
    for(int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
        {
            this->stars[i*10+j] = Star(i*0.1 -0.5, j*0.1 -0.5, 2*pow(10, 30));
        }
    
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
        this->stars[star_id].apply_force(this->stars[i].force_field(this->stars[star_id].position - this->stars[i].position), this->time_step);
        std::cout << this->stars[i].force_field(this->stars[star_id].position - this->stars[i].position).x << std::endl;
    }
}

void Galaxy::Update(){
    const float dt = 0.001;
    for(int i = 0; i < this->star_count; i++)
        this->calculate_force(i);
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
