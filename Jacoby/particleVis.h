#pragma once
/*
Class: paricleVis
Visualization of particles.
*/
#ifndef _PARTICLEVIS_
#define _PARTICLEVIS_

#ifndef _GLM_STUFF_
#define _GLM_STUFF_
#include <glm/glm/glm.hpp>	// API for linear algebra operations compatible with OpenGL
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#endif

#include "shader.h"
#include "Inc/jacoby/types.h"
#include "Inc/jacoby/particle.h"

class particleVis : public jacoby::Particle<FLOAT> {
public:
	typedef jacoby::Particle<FLOAT> particleBase;

private:
	static const float vertices[];			// vertices of the model (for the VBO, OpenGL)
	static const unsigned int indices[];	// indices of the model (for the EBO, OpenGL)
	static unsigned int VAO;				// vertex array object id
	static unsigned int EBO;				// element buffer object id
	static unsigned int VBO;				// vertex buffer object id
	static unsigned int count;				// number of objects
	static Shader m_SO;

	glm::mat4 modelMatrix;					// model matrix		
	unsigned int state;						// state (how many hits to destroy)
	glm::vec3 position;						// position of a brick

	particleVis() {}; // make private
public:

	particleVis(const glm::vec3 _pos);
	particleVis& operator = (const particleVis&) = default;

	static void InitParticleShader();

	// getters and setters
	void set_position(const glm::vec3&);
	void update_position(const float&);
	glm::vec3 get_position() const;
	static unsigned int get_count();

	// other methods
	void prepare_to_draw();
	void draw();
};

#endif
