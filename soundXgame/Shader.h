#ifndef __SHADER_H__
#define __SHADER_H__

#include "projectGrafics.h"
#include <stdio.h>
#include <stdlib.h>

#include "Connectable.h"
#include <math.h>


class Shader :
	public IConnectable
{
private:
public:
	GLuint program;
	// Attribute + Uniforms f�r Vertices und Farbe
	GLint attribute_coord3d, attribute_v_color;
	//GLint uniform_alpha;
	// Buffer f�r Vertices und Farben
	//GLuint vbo_triangle, vbo_triangle_colors;
	
	bool initResources(const char* filenameVertex, const char* filenameFragment);
	GLuint createShader(const char* filename, GLenum type);
	char* readFile(const char* filename);
	void freeResources();
protected:
	void drawBegin(void);
	void drawEnd(void);
	bool setVariable(const char* variableName, float x, float y, float z, float w);
public:
	Shader(void);
	~Shader(void);


};

#endif