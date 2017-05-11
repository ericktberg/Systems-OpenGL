#include "Program.h"
#include <iostream>

Program::Program(int type) {
	program_id_ = glCreateProgram();
	std::string f;
	if (type == CONSTANT_COL) {
		f = "shaders/Fragment/simpleFragment.glsl";
	}
	else if (type == PHONG_NOSPEC)	{
		f = "shaders/Fragment/diffuse.glsl";
	}
	Shader fragment(f, GL_FRAGMENT_SHADER);

	Shader vertex("shaders/Vertex/simple2d.glsl", GL_VERTEX_SHADER);

	glAttachShader(program_id_, vertex.id());
	glAttachShader(program_id_, fragment.id());

	glBindFragDataLocation(program_id_, 0, "outColor");

	glLinkProgram(program_id_);
	glUseProgram(program_id_);

	GLint status;
	glGetProgramiv(program_id_, GL_LINK_STATUS, &status);
	if (!status) {
		char buffer[512];
		GLsizei len;
		glGetProgramInfoLog(program_id_, 512, NULL, buffer);
		fprintf(stderr, "%s\n", buffer);
	}
}

Program::~Program() {
	glDeleteProgram(program_id_);
}

void Program::setUniformMatrix4fv(const GLchar* uniform, const GLfloat* data) {
	glUniformMatrix4fv(getUniformLoc(uniform), 1, GL_FALSE, data);
}

void Program::setUniformVector3fv(const GLchar* uniform, float v0, float v1, float v2) {
	glUniform3f(getUniformLoc(uniform), v0, v1, v2);
}