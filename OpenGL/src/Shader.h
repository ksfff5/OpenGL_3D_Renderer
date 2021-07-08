#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>
#include <string>
#include <iostream>
#include <fstream>

//TODO:
//- Add functions for updating uniforms
//- Add function for determining if this program is currently in use

//When adding more shaders to this class update:
//Constructor
//Destructor

//A class for handling Program and Shader objects
class Shader
{
	private:
		//Read in a shader from a given file and return the source code. Returns 0 if failed to open/read file
		std::string readShaderFile(const char* shaderFile);

		//Create/Compile a shader, returns 0 if failed to compile successfully, else returns shader ID
		int CreateShader(GLenum shaderType, const char* shaderCode);

		//ID of the program object
		unsigned int programID;

		//whether or not the shaders compiled and linked correctly
		bool usable;

		//ID of vertex shader. Needed for deletion
		unsigned int vertexShaderID;

		//ID of fragment shader. Needed for deletion
		unsigned int fragmentShaderID;

	public:
		//Constructor that takes in file names for the shader programs
		Shader(std::string vertexShaderFile, std::string fragmentShaderFile);

		//Destructor
		~Shader();

		//If this program is usable, set it as the active program in open GL. Returns whether or not that was done.
		bool Use();

		//Sets a specific float uniform to a specifc value. Will switch to having this program object to be in use (if possible). Returns whether or not setting the uniform was successful.
		bool setUniformFloat(const char* name, float value);

		//Getter for programID
		unsigned int GetProgramID();

		//Getter for usable
		bool IsUsable();
};

#endif