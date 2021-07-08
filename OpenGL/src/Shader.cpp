#include "Shader.h"

//Read in a shader from a given file and return the source code. Returns 0 if failed to open/read file
std::string Shader::readShaderFile(const char* shaderFile)
{
	std::string shaderCode = "";
	std::ifstream fileIn(shaderFile);

	if (!fileIn.is_open())
	{
		//if the file failed to open return "#failed"
		return "#failed_to_read_file";
	}

	while (!fileIn.eof())
	{
		shaderCode += fileIn.get();
	}

	//Remove the character at the end of the string that itnerferes with compiling (probably the eof character)
	shaderCode.pop_back();
	fileIn.close();
	return shaderCode;
}

//Create/Compile a shader, returns 0 if failed to compile successfully, else returns shader ID
int Shader::CreateShader(GLenum shaderType, const char* shaderCode)
{
	//If we couldn't read one of the shaders, dont try to compile it
	if (std::string(shaderCode) == std::string("#failed_to_read_file"))
	{
		std::cout << "Error: Failed to open/read file for one of the shaders" << std::endl;
		return 0;
	}

	//Create Shader and Compile the shader code
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	//Check that the shader compiled correctly
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Error: shader failed to compile: " << infoLog << std::endl;
		return 0;
	}
	return shader;
}

//Constructor that takes in file names for the shader programs
Shader::Shader(std::string vertexShaderFile, std::string fragmentShaderFile)
{
	usable = false;

	//Create all our shaders and our program
	vertexShaderID = CreateShader(GL_VERTEX_SHADER, readShaderFile(vertexShaderFile.c_str()).c_str());
	fragmentShaderID = CreateShader(GL_FRAGMENT_SHADER, readShaderFile(fragmentShaderFile.c_str()).c_str());

	programID = glCreateProgram();

	if (vertexShaderID != 0 && fragmentShaderID != 0)
	{
		//If our shaders were created properly, attach and link them
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);

		//Check link status
		int linkStatus;
		glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_FALSE)
		{
			//Print linking erros if necessary
			char infoLog[512];
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			std::cout << "Error, Program failed to link: " << infoLog << std::endl;
		}
		else
		{
			//If we link properly, then the program is usable
			usable = true;
		}
	}
}

//Destructor
Shader::~Shader()
{
	//Deleting the program implicitly detaches all shaders
	glDeleteProgram(programID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

//If this program is usable, set it as the active program in open GL. Returns whether or not that was done.
bool Shader::Use()
{
	if (usable == true)
	{
		glUseProgram(programID);
	}
	return usable;
}

//Sets a specific float uniform to a specifc value. Will switch to having this program object to be in use (if possible). Returns whether or not setting the uniform was successful.
bool Shader::setUniformFloat(const char* name, float value)
{
	if (Use())
	{
		int location = glGetUniformLocation(programID, name);
		if (location != -1)
		{
			glUniform1f(location, value);
			return true;
		}
	}
	return false;
}

//Getter for programID
unsigned int Shader::GetProgramID()
{
	return programID;
}

//Getter for usable
bool Shader::IsUsable()
{
	return usable;
}