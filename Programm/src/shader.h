#pragma once

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertex_shader_path, const char* fragment_shader_path);

	void use();

	void set_bool(const char* name, bool value) const;
	void set_int(const char* name, int value) const;
	void set_float(const char* name, float value) const;
};