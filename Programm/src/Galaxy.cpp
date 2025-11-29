#include "Galaxy.h"

Galaxy::Galaxy(){
    // Init list of stars
    this->star_count = 100;
    this->stars = new Vector2[this->star_count];
    for(int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
        this->stars[i*10+j] = Vector2(i*0.1 -0.5, j*0.1 -0.5);
    
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

void Galaxy::Update(){
    const float dt = 0.001;
    for(int i = 0; i < this->star_count; i++)
        this->stars[i] = Vector2(this->stars[i].x, this->stars[i].y -dt);
}
void Galaxy::Draw(){
    for(int i = 0; i < this->star_count; i++)
    {
        this->shader.use();
        shader.set_vec2("offset", this->stars[i]);
        glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
