#include "Galaxy.h"

Galaxy::Galaxy(){
    // Init list of stars
    this->stars = new Vector2[this->star_count];
    this->stars[0] = Vector2(0, 0);
    
    // Init shader
	this->shader = Shader("Shader/star_vert.shader", "Shader/star_frag.shader");
    //Create the star object for rendering
    const int nb_pts = 60
	float vertices[nb_pts*2];// Array holdind the vertices of the circle
    for(int i = 0; i < nb_pts*2; i++)
    {
        vertices[i]   = this->star_radius * cos(i * 2*PI/nb_pts);
        vertices[i+1] = this->star_radius * sin(i * 2*PI/nb_pts);
    }
	glGenVertexArrays(1, &this->VA0);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(this->VA0); // Bind the VAO first

	// Then configure the VBO
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Finaly configure the VA0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * nb_pts * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
	glBindVertexArray(0); // Unbind the VA0
}

Galaxy::~Galaxy(){
    delete this->stars;
}

void Galaxy::Update(){

}
void Galaxy::Draw(){
    
}
