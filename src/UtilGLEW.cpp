#include "util/UtilGLEW.h"
#include <vector>

/* Read a file into string */
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

void compileShaders(GLuint* vertex_shader, GLuint* fragment_shader) {
	GLint status;

	/* Read shaders from file */
	std::string vertexSource = readFile("shaders/Vertex/simple3d.glsl");
	std::string fragmentSource = readFile("shaders/Fragment/simpleFragment.glsl");
	const char *vertCStr = vertexSource.c_str();
	const char *fragCStr = fragmentSource.c_str();
	/* Compile vertex Shader */
	*vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(*vertex_shader, 1, &vertCStr, NULL);
	glCompileShader(*vertex_shader);
	// Check Compilation for Errors
	glGetShaderiv(*vertex_shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(*vertex_shader, 512, NULL, buffer);
		fprintf(stderr, "%s\n", buffer);
		exit(EXIT_FAILURE);
	}

	/* Compile fragment Shader*/
	*fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(*fragment_shader, 1, &fragCStr, NULL);
	glCompileShader(*fragment_shader);
	// Check Compilation for Errors
	glGetShaderiv(*fragment_shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(*vertex_shader, 512, NULL, buffer);
		fprintf(stderr, "%s\n", buffer);
		exit(EXIT_FAILURE);
	}
}

/* Combine Shaders into program */
void programShaders(GLuint* shader_program, const GLuint& vertex_shader, const GLuint& fragment_shader) {
	*shader_program = glCreateProgram();
	glAttachShader(*shader_program, vertex_shader);
	glAttachShader(*shader_program, fragment_shader);
	glBindFragDataLocation(*shader_program, 0, "outColor");

	/* Link Shaders for use */
	glLinkProgram(*shader_program);
	glUseProgram(*shader_program);
}