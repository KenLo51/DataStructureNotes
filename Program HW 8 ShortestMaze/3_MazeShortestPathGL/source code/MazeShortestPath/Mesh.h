#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>

// Vertex with all parameters
typedef struct VERTEX_TYPEDEF{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec4 color;
	glm::vec2 texCoord;
	glm::vec4 tangent;

	bool operator ==(const struct VERTEX_TYPEDEF& a);

	// void operator =(const struct VERTEX_TYPEDEF& a); compiler will generate
	void operator =(const struct VERTEX_3P2T_TYPEDEF& a); 
	void operator =(const struct VERTEX_3P3N4C_TYPEDEF& a);
	void operator =(const struct VERTEX_3P3N2T_TYPEDEF& a);
}Vertex;
// with position(vec3) and texture-coordinate(vec2)
typedef struct VERTEX_3P2T_TYPEDEF {
	glm::vec3 position;
	glm::vec2 texCoord;

	bool operator ==(const struct VERTEX_3P2T_TYPEDEF& a);

	void operator =(const struct VERTEX_TYPEDEF& a);
	// void operator =(const struct VERTEX_3P2T_TYPEDEF& a); compiler will generate
	void operator =(const struct VERTEX_3P3N4C_TYPEDEF& a);
	void operator =(const struct VERTEX_3P3N2T_TYPEDEF& a);
}Vertex3P2T;
// Vertex with position, nornal vector and color
typedef struct VERTEX_3P3N4C_TYPEDEF {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec4 color;

	bool operator ==(const struct VERTEX_3P3N4C_TYPEDEF& a);

	void operator =(const struct VERTEX_TYPEDEF& a);
	void operator =(const struct VERTEX_3P2T_TYPEDEF& a);
	// void operator =(const struct VERTEX_3P3N4C_TYPEDEF& a); compiler will generate
	void operator =(const struct VERTEX_3P3N2T_TYPEDEF& a);
}Vertex3P3N4C;
// Vertex with position, nornal vector and color
typedef struct VERTEX_3P3N2T_TYPEDEF {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec4 color;

	bool operator ==(const struct VERTEX_3P3N4C_TYPEDEF& a);

	void operator =(const struct VERTEX_TYPEDEF& a);
	void operator =(const struct VERTEX_3P2T_TYPEDEF& a);
	void operator =(const struct VERTEX_3P3N4C_TYPEDEF& a);
	// void operator =(const struct VERTEX_3P3N2T_TYPEDEF& a); compiler will generate
}Vertex3P3N2T;

class Mesh {
public:
	const static unsigned int VERTEXTYPE_ALL;
	const static unsigned int VERTEXTYPE_3P2T;
	const static unsigned int VERTEXTYPE_3P3N4C;
	const static unsigned int VERTEXTYPE_3P3N2T;

	const static unsigned int LAYOUTINDEX_POSITION;
	const static unsigned int LAYOUTINDEX_NORMAL;
	const static unsigned int LAYOUTINDEX_COLOR;
	const static unsigned int LAYOUTINDEX_TEXCOORD;
	const static unsigned int LAYOUTINDEX_TANGENT;

public:
	GLuint vbo_ID, vao_ID, ebo_ID;
	unsigned int vertexType;

	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
public:
	// Mesh constructor
	Mesh();
	Mesh(std::vector <Vertex>& vertices, unsigned int vertexType = Mesh::VERTEXTYPE_ALL);
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, unsigned int vertexType = Mesh::VERTEXTYPE_ALL);

	// build VAO VBO EBO for openGL by vertices and indices
	void init();

	// load vertices and indices
	void Load(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, unsigned int vertexType = Mesh::VERTEXTYPE_ALL);
	void Load(std::vector <Vertex>& vertices, unsigned int vertexType = Mesh::VERTEXTYPE_ALL);

	// draw mesh by indices array(EBO)
	void Draw(GLenum mode = GL_TRIANGLES);
	// draw mesh by indices of each vertex in VAO (without VEO)
	void DrawArray(GLenum mode = GL_TRIANGLES);

	// delete mesh
	void Delete();
};

void basicMesh_Plane(Mesh* mesh, glm::vec2 scale = glm::vec2(1.0f, 1.0f));

#endif