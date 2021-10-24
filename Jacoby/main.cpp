#include <iostream>

#ifndef _GLAD_
#define _GLAD_
#include <glad/glad.h>	// API for pointers in OpenGL
#endif
#ifndef _GLFW3_
#define _GLFW3_
#include <glfw3.h>		// API for context creation
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include "particleVis.h"
#include "Inc/jacoby/pfgen.h"

#define PARTICLE_NUM 10

using namespace std;

void framebuffer_size_callback(GLFWwindow*, int, int); // a function that will resize the viewport when window size changes

void processInput(GLFWwindow*); // a process that handles pressing of a key

// Here we will draw a simple triangle with vertices (-0.5,-0.5, 0.0), (-0.5, 0.5, 0.0), (0.5, 0.0, 0.0)

int main() {

	//	--------------------------------------------------------------------------------------------------------------------
	//	--------------------------------------------------------------------------------------------------------------------
	//	--------------------------------------------------------------------------------------------------------------------

	//	--------------------------------------------------------------------------------------------------------------------
	// Here starts window and context initialization
	glfwInit(); // Initializes GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// OpenGL version 3.*
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// OpenGL version *.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGL use core profile
	// Now we create a window (800x600 pixels)
	GLFWwindow* window = glfwCreateWindow(1600, 900, "JacobyTest", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to open window.\n";
		return 1;
	}
	// Finally we make the context of the window the main context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// now try to initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD.\n";
		return 1;
	}

	jacoby::ParticleForceManager fMan;

	particleVis::InitParticleShader();
	std::vector< particleVis > particles;
	jacoby::ParticleGravity testPartGrav(jacoby::Vector3<FLOAT>(0.0f, -10.0f, 0.0f));
	jacoby::ParticleDrag testPartDrag(0.05f, 0.05f);
	// first create particles
	for (int ind = 0; ind <= PARTICLE_NUM; ++ind)
	{
		particles.push_back(particleVis(glm::vec3(-10.0f + 2.0f * (float)ind, 0.0f, 0.0f)));
		particles[ind].SetVelocity(jacoby::Vector3<FLOAT>(0.0f, 0.0f, 0.0f));
	}
	// then apply forces, as pointers to said particles need to be well defined
	// generally using vectors in here is not very safe - better use vector of pointers
	for (int ind = 0; ind <= PARTICLE_NUM; ++ind)
	{
		if (ind > 0)
		{
			jacoby::ParticleSpring* newSpring;
			newSpring = new jacoby::ParticleSpring((jacoby::Particle<FLOAT>*)(&(particles[ind - 1])), 20.0f, 0.0f);
			fMan.Add((jacoby::Particle<FLOAT>*)(&(particles[ind    ])), newSpring);
			newSpring = new jacoby::ParticleSpring((jacoby::Particle<FLOAT>*)(&(particles[ind    ])), 20.0f, 0.0f);
			fMan.Add((jacoby::Particle<FLOAT>*)(&(particles[ind - 1])), newSpring);
		}
		fMan.Add((jacoby::Particle<FLOAT>*)(&(particles[ind])), &testPartGrav);
		fMan.Add((jacoby::Particle<FLOAT>*)(&(particles[ind])), &testPartDrag);
	}


	particleVis testPart(glm::vec3(0.0f, 0.0f, 0.0f));
	testPart.SetVelocity(jacoby::Vector3<FLOAT>(10.0f, 0.0f, 0.0f));
	//testPart.SetAcceleration(jacoby::Vector3<FLOAT>(0.0f, -10.0f, 0.0f));

	jacoby::Vector3<FLOAT> anchor(0.0f, 0.0f, 0.0f);
	jacoby::ParticleAnchoredSpring testPartAnchSpr(&anchor, 8.0f, 0.0f);
	jacoby::Vector3<FLOAT> anchor2(-10.0f, 0.0f, 0.0f);
	jacoby::ParticleAnchoredSpring testPartAnchSpr2(&anchor2, 40.0f, 0.0f);
	jacoby::Vector3<FLOAT> anchor3(10.0f, 0.0f, 0.0f);
	jacoby::ParticleAnchoredSpring testPartAnchSpr3(&anchor3, 40.0f, 0.0f);

	fMan.Add((jacoby::Particle<FLOAT>*)(&(particles[0])), &testPartAnchSpr2);
	fMan.Add((jacoby::Particle<FLOAT>*)(&(particles[PARTICLE_NUM])), &testPartAnchSpr3);

	//	--------------------------------------------------------------------------------------------------------------------
	// render loop - keep drawing until we want to end
	float oldTimeVal = glfwGetTime();
	float oldY = testPart.get_position().y;
	while (!glfwWindowShouldClose(window)) {
		processInput(window);	// process any input that has occured

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// set the default color to which the screen is reset
		glClear(GL_COLOR_BUFFER_BIT);	// clear the screen

		float timeVal = glfwGetTime();

		testPart.prepare_to_draw();
		testPart.draw();

		for (int ind = 0; ind <= PARTICLE_NUM; ++ind)
		{
			particles[ind].prepare_to_draw();
			particles[ind].draw();
		}

		float deltaTime = timeVal - oldTimeVal;
		oldTimeVal = timeVal;
		testPart.update_position(deltaTime);
		testPartGrav.UpdateForce(&testPart, deltaTime);
		testPartAnchSpr.UpdateForce(&testPart, deltaTime);

		for (int ind = 0; ind <= PARTICLE_NUM; ++ind)
		{
			particles[ind].update_position(deltaTime);
			fMan.UpdateForces(deltaTime);
		}
		/*
		if (testPart.get_position().y <= 0.0f && oldY > 0.0f)
		{
			auto& velY = testPart.GetVelocity().getY();
			velY *= -0.8f;
		}
		oldY = testPart.get_position().y;
		*/

		glfwSwapBuffers(window);	
		glfwPollEvents();	
	}	// end of rendering loop
	//	--------------------------------------------------------------------------------------------------------------------

	//at the end we want to properly clean the resources
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow*, int width, int height) { // a function that will resize the
	glViewport(0, 0, width, height);
}

// a process that handles pressing of a key
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // if the escape key was pressed then
		glfwSetWindowShouldClose(window, true);	// set the variable for window closing to true
	}
}