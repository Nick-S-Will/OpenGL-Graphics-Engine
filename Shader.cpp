#include "Shader.h"

Shader::~Shader()
{
	Cleanup();
}

void Shader::LoadShaders(const char* vertexFilePath, const char* fragmentFilePath)
{
	CreateShaderProgram(vertexFilePath, fragmentFilePath);
	LoadAttributes();
}

void Shader::SetVec3(const char* name, glm::vec3 value)
{
	GLint location = glGetUniformLocation(programID, name);
	if (location == -1) return;

	glUniform3fv(location, 1, &value[0]);
}

void Shader::SetMat4(const char* name, glm::mat4 value)
{
	GLint location = glGetUniformLocation(programID, name);
	if (location == -1) return;

	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void Shader::SetFloat(const char* name, float value)
{
	GLint location = glGetUniformLocation(programID, name);
	if (location == -1) return;

	glUniform1f(location, value);
}

void Shader::Cleanup()
{
	if (programID != 0)
	{
		glDeleteProgram(programID);
	}
	programID = 0;
}

void Shader::CreateShaderProgram(const char* vertexFilePath, const char* fragmentFilePath)
{
	programID = glCreateProgram();
	GLuint vertexShaderID = LoadShaderFile(vertexFilePath, GL_VERTEX_SHADER);
	GLuint fragmentShaderID = LoadShaderFile(fragmentFilePath, GL_FRAGMENT_SHADER);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	EvaluateShader(infoLogLength, programID);

	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

GLuint Shader::LoadShaderFile(const char* filePath, GLenum type)
{
	GLuint shaderID = glCreateShader(type);

	std::string shaderCode;
	std::ifstream shaderStream(filePath, std::ios::in);
	M_ASSERT(shaderStream.is_open(), ("Imposible to open %s.\n", filePath));
	std::string line = "";
	while (getline(shaderStream, line)) shaderCode += "\n" + line;
	shaderStream.close();

	const char* sourcePointer = shaderCode.c_str();
	glShaderSource(shaderID, 1, &sourcePointer, NULL);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	EvaluateShader(infoLogLength, shaderID);

	glAttachShader(programID, shaderID);

	return shaderID;
}

void Shader::LoadAttributes()
{
	verticesLocation = glGetAttribLocation(programID, "vertices");
	normalsLocation = glGetAttribLocation(programID, "normals");
	textureCoordsLocation = glGetAttribLocation(programID, "texCoords");
	sampler1Location = glGetUniformLocation(programID, "sampler1");
	sampler2Location = glGetUniformLocation(programID, "sampler2");
}

void Shader::EvaluateShader(int infoLength, GLuint id)
{
	if (infoLength > 0) {
		std::vector<char> errorMessage(infoLength + 1);
		glGetShaderInfoLog(id, infoLength, NULL, &errorMessage[0]);
		M_ASSERT(0, ("%s\n", &errorMessage[0]));
	}
}
