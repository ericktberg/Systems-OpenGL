#include "Shader.h"
#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

std::string readFile(const char* file_path) {
	std::string shader_content;
	std::ifstream file_stream(file_path, std::ios::in);

	std::string line = "";

	if (!file_stream.is_open()) {
		std::cerr << "Could not read file " << file_path << ". File does not exist." << std::endl;
		return line.c_str();
	}

	while (!file_stream.eof()) {
		std::getline(file_stream, line);
		shader_content.append(line + "\n");
	}

	file_stream.close();
	return shader_content;
}

Shader::Shader(std::string source, GLenum type) {
	std::string vertexSource = readFile(source.c_str());
	const GLchar *cstr = vertexSource.c_str();
	fprintf(stderr, "%s", cstr);
	shader_id_ = glCreateShader(type);
	glShaderSource(shader_id_, 1, &cstr, NULL);
	glCompileShader(shader_id_);
	GLint status;
	glGetShaderiv(shader_id_, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(shader_id_, 512, NULL, buffer);
		fprintf(stderr, "%s\n", buffer);

		throw std::runtime_error(buffer);
	}
}

Shader::~Shader() {
	glDeleteShader(shader_id_);
}