#include "Program.h"

Program::Program(std::vector<Shader> shaders) : shaders_(shaders) {
	program_id_ = glCreateProgram();
	for (int i = 0; i < shaders_.size(); i++) {
		glAttachShader(program_id_, shaders_.at(i).id());
	}

	glBindFragDataLocation(program_id_, 0, "outColor");

	glLinkProgram(program_id_);
	glUseProgram(program_id_);
}

Program::~Program() {
	glDeleteProgram(program_id_);
}

void Program::setUniformMatrix4fv(const GLchar* uniform, const GLfloat* data) {
	glUniformMatrix4fv(getUniformLoc(uniform), 1, GL_FALSE, data);
}