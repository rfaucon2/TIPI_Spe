#include "shader.h"
Vector2::Vector2(){}
Vector2::Vector2(double x, double y)
{
    this->x = x;
    this->y = y;
}
void Vector2::operator+=(Vector2 v)
{
    this->x += v.x;
    this->y += v.y;
}

Vector2 operator-(Vector2 u, Vector2 v){
    return Vector2(u.x - v.x, u.y -v.y);
}
Vector2 operator*(double a, Vector2 v){
    return Vector2(a*v.x, a*v.y);
}

double dist(Vector2 v, Vector2 w){
    return sqrt(pow(v.x - w.x, 2) + pow(v.y - w.y, 2));
}

Shader::Shader(){}
Shader::Shader(const char* vertex_shader_path, const char* fragment_shader_path)
{

	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertex_shader_path);
		fShaderFile.open(fragment_shader_path);
		std::stringstream vShaderStream, fShaderStream;
		// read file’s buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << e.what() << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	};

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	};

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
			infoLog << std::endl;
	}
	// delete shaders; they’re linked into our program and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::set_bool(const char* name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name), (int)value);
}

void Shader::set_int(const char* name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::set_float(const char* name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::set_vec2(const char* name, Vector2 value) const
{
   glUniform2f(glGetUniformLocation(ID, name), value.x, value.y); 
}
