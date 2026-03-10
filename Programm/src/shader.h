#pragma once

#include <glad/glad.h>
#include <random>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

class Vector2{
public:
    double x, y;
    Vector2();
    Vector2(double x, double y);
    
    void operator+=(Vector2 v);
};
Vector2 operator*(double a, Vector2 v);
Vector2 operator/(Vector2 v, double a);
Vector2 operator+(Vector2 u, Vector2 v);
Vector2 operator-(Vector2 u, Vector2 v);
double dist(Vector2 v, Vector2 w);

class Shader
{
public:
	unsigned int ID;
    Shader();
	Shader(const char* vertex_shader_path, const char* fragment_shader_path);

	void use();

	void set_bool(const char* name, bool value)     const;
	void set_int(const char* name, int value)       const;
	void set_float(const char* name, float value)   const;
    void set_vec2(const char*, Vector2 value)       const;
};
