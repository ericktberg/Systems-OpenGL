#include "Program.h"
#include <iostream>

Program::Program(std::vector<Shader> shaders) {
	program_id_ = glCreateProgram();
	/*for (int i = 0; i < shaders_.size(); i++) {
		GLuint id = shaders_.at(i).id();
		glAttachShader(program_id_, id);
	}*/

	Shader vertex("shaders/Vertex/simple3d.glsl", GL_VERTEX_SHADER);
	Shader fragment("shaders/Fragment/simpleFragment.glsl", GL_FRAGMENT_SHADER);

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