/* Now we create a shader class that will read the GLSL code, compile it, initialize shaders, activate shaders, ...*/
#pragma once 
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#ifndef SHADER_H
#define SHADER_H

class Shader {
public:
	unsigned int ID;

	Shader() { ID = 0; }
	Shader(const char* vertexPath, const char* fragmentPath);	// constructor reads file under paths and creates shaders

	void use();	// for using or activating shader

	// functions for uniform setters
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;


};
#endif
