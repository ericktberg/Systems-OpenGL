/* inspiration for this include chain 
 * http://www.tomdalling.com/blog/modern-opengl/05-model-assets-and-instances/
*/

#pragma once

#include <GL/glew.h>
#include "Shader.h"

#include <vector>
#include <string>

class Program {

public:
	Program(int type);
	~Program();
	void setUniformMatrix4fv(const GLchar* uniform, const GLfloat* data);
	void setUniformVector3fv(const GLchar* unfirom, float v0, float v1, float v2);
	void bind() const { glUseProgram(program_id_); }

	GLint getAttribLoc(const GLchar* attrib) { return glGetAttribLocation(program_id_, attrib); }
	GLint getUniformLoc(const GLchar* uniform) { return glGetUniformLocation(program_id_, uniform); }
	GLuint id() { return program_id_; }
private:
	GLuint program_id_;

	std::vector<Shader> shaders_;
};