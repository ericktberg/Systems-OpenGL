#pragma once
#include <GL/glew.h>
#include <string>

class Shader {
public:
	Shader(std::string, GLenum);
	~Shader();

	GLuint id() const { return shader_id_; }

private:
	GLuint shader_id_;
	std::string shader_text_;
};