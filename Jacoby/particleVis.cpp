#include "particleVis.h"
//#include "shader.h"

#ifndef _GLAD_
#define _GLAD_
#include <glad/glad.h>	// API for pointers in OpenGL
#endif
#ifndef _GLFW3_
#define _GLFW3_
#include <glfw3.h>		// API for context creation
#endif
const float particleVis::vertices[] = {
	//back side
		-0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f, -0.1f,
		 0.1f,  0.1f, -0.1f,
		-0.1f,  0.1f, -0.1f,
		//bottom side
		-0.1f, -0.1f,  0.1f,
		 0.1f, -0.1f,  0.1f,
		 0.1f, -0.1f, -0.1f,
		-0.1f, -0.1f, -0.1f,
		//right side
		 0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f,  0.1f,
		 0.1f,  0.1f,  0.1f,
		 0.1f,  0.1f, -0.1f,
		 //top side
		 -0.1f,  0.1f, -0.1f,
		  0.1f,  0.1f, -0.1f,
		  0.1f,  0.1f,  0.1f,
		 -0.1f,  0.1f,  0.1f,
		 // left side
		 -0.1f, -0.1f,  0.1f,
		 -0.1f, -0.1f, -0.1f,
		 -0.1f,  0.1f, -0.1f,
		 -0.1f,  0.1f,  0.1f,
		 //front side
		  -0.1f, -0.1f,  0.1f,
		   0.1f, -0.1f,  0.1f,
		   0.1f,  0.1f,  0.1f,
		  -0.1f,  0.1f,  0.1f
};

// We draw the object using vertices combined into triangles.
// Indices specify which indices to combine into which triangle.
const unsigned int particleVis::indices[] = {
		0, 1, 3,
		1, 2, 3,
		4, 5, 7,
		5, 6, 7,
		8, 9, 11,
		9, 10, 11,
		12, 13, 15,
		13, 14, 15,
		16, 17, 19,
		17, 18, 19,
		20, 21, 23,
		21, 22, 23
};

// Vertex array object (VAO), element buffer object (EBO)
// and vertex buffer object (VBO) are shared between all of the objects
// of type "particleVis"
// Here are ids of these objects
unsigned int particleVis::VAO = 0;
unsigned int particleVis::EBO = 0;
unsigned int particleVis::VBO = 0;

// We keep count of the number of brick objects through count
unsigned int particleVis::count = 0;

Shader particleVis::m_SO;

particleVis::particleVis(const glm::vec3 _pos) {
	// keep count of number of bricks
	++count;

	// set the position and state
	set_position(_pos);
	position = _pos;

	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);

	// set VAO, EBO, VBO if needed
	if (count == 1) {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void particleVis::InitParticleShader()
{
	m_SO = Shader("./vertexShaderParticle.vert", "./fragmentShaderParticle.frag");
	m_SO.use();
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int tView = glGetUniformLocation(m_SO.ID, "view");
	glUniformMatrix4fv(tView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 600.0f / 600.0f, 0.1f, 100.0f);
	unsigned int tProj = glGetUniformLocation(m_SO.ID, "proj");
	glUniformMatrix4fv(tProj, 1, GL_FALSE, glm::value_ptr(proj));
}

// -------------------------------------------------------------
// Setters and getters
	// setter position
void particleVis::set_position(const glm::vec3& _position) {
	particleBase::VectorType vec(_position.x, _position.y, _position.z);
	SetPosition(vec);

	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
}

// updates position based on the 
void particleVis::update_position(const float& deltaT) {
	Integrate(deltaT);
	position.x = GetPosition().getX();
	position.y = GetPosition().getY();
	position.z = GetPosition().getZ();

	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
}

// getter position
glm::vec3 particleVis::get_position() const {
	particleBase::VectorType vec = Position();
	return glm::vec3(vec.getX(), vec.getY(), vec.getZ());
}

// Get the count of bricks
unsigned int particleVis::get_count() {
	return count;
}

// -------------------------------------------------------------
// Other class methods

	//Prepare OpenGL to draw object of this type
void particleVis::prepare_to_draw() {
	m_SO.use();
	glBindVertexArray(VAO);
}

// Tells OpenGL to draw object of this type
void particleVis::draw() {
	unsigned int modelID = glGetUniformLocation(m_SO.ID, "model");
	glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
