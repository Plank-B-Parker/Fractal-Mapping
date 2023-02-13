#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

struct complex;


class Shader
{
private:
	unsigned int programID;
	unsigned int vertexID;
	unsigned int fragID;

	const char* vertexShader;
	const char* fragShader;

	void ParseShader(const char* vertexPath, const char* fragPath);
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	void throwShade() const;
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setComplex(const std::string& name, complex value) const;
};
