#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
/* GLEW stuff */
void initGLEW();

/* Create Vertex Array Object*/
void createVAO(GLuint* vao);

/* Create Buffer Object for communing with dat yung GPU */
void createXBO(GLuint* xbo, const std::vector<GLfloat> buffer_name);

/* Compiles shaders. Exits on bad shaders */
void compileShaders(GLuint* vertex_shader, GLuint* fragment_shader);

/* Combine Shaders into program */
void programShaders(GLuint* shader_program, const GLuint& vertex_shader, const GLuint& fragment_shader);

/* Read a file into string */
std::string readFile(const char* file_path);