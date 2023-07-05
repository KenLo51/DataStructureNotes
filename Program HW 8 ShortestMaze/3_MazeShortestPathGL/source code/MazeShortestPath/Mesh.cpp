#include "Mesh.h"

// Vertex operators /////////////////////////////////////////////////////////////////////////////
bool Vertex::operator ==(const struct VERTEX_TYPEDEF& a) {
	if (position != a.position) return false;
	if (normal != a.normal) return false;
	if (color != a.color) return false;
	if (texCoord != a.texCoord) return false;
	if (tangent != a.tangent) return false;
	return true; 
}
void Vertex::operator =(const struct VERTEX_3P2T_TYPEDEF& a) {
	position = a.position;
	normal = glm::vec3(0.0f, 1.0f, 0.0f);
	color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	texCoord = a.texCoord;
	tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

// constant variables ///////////////////////////////////////////////////////////////////////////
const unsigned int Mesh::VERTEXTYPE_ALL = 0;
const unsigned int Mesh::VERTEXTYPE_3P2T = 1;
const unsigned int Mesh::VERTEXTYPE_3P3N4C = 2;
const unsigned int Mesh::VERTEXTYPE_3P3N2T = 3;

const unsigned int Mesh::LAYOUTINDEX_POSITION = 0;
const unsigned int Mesh::LAYOUTINDEX_NORMAL = 1;
const unsigned int Mesh::LAYOUTINDEX_COLOR = 2;
const unsigned int Mesh::LAYOUTINDEX_TEXCOORD = 3;
const unsigned int Mesh::LAYOUTINDEX_TANGENT = 4;

// Mesh class functions ////////////////////////////////////////////////////////////////
// Mesh constructor
Mesh::Mesh() {
	Mesh::vao_ID = 0;
	Mesh::vbo_ID = 0;
	Mesh::ebo_ID = 0;

	Mesh::vertexType = Mesh::VERTEXTYPE_ALL;
}
Mesh::Mesh(std::vector <Vertex>& vertices, unsigned int vertexType) {
	Mesh::vao_ID = 0;
	Mesh::vbo_ID = 0;
	Mesh::ebo_ID = 0;
	Mesh::vertexType = vertexType;

	Mesh::Load(vertices, indices);
}
Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, unsigned int vertexType) {
	Mesh::vao_ID = 0;
	Mesh::vbo_ID = 0;
	Mesh::ebo_ID = 0;
	Mesh::vertexType = vertexType;

	Mesh::Load(vertices, indices);
}
void Mesh::init() {
	// generate buffers
	glGenVertexArrays(1, &(Mesh::vao_ID));
	glGenBuffers(1, &(Mesh::vbo_ID));
	glGenBuffers(1, &(Mesh::ebo_ID));

	glBindVertexArray(Mesh::vao_ID);

	// Vertex Buffer Object
	// bind VBO into VAO
	glBindBuffer(GL_ARRAY_BUFFER, Mesh::vbo_ID);
	//		copy vertex data to VBO
	glBufferData(GL_ARRAY_BUFFER, Mesh::vertices.size() * sizeof(Vertex), Mesh::vertices.data(), GL_STATIC_DRAW);

	// Element Buffer Object
	//		bind EBO into VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh::ebo_ID);
	//		copy data to EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Mesh::indices.size() * sizeof(GLuint), Mesh::indices.data(), GL_STATIC_DRAW);

	//		setting attributes & register VBO
	//		typedef struct {
	//			glm::vec3 position;
	//			glm::vec3 normal;
	//			glm::vec4 color;
	//			glm::vec2 texCoord;
	//		}Vertex;
	//		link position
	//		void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	if (Mesh::vertexType == Mesh::VERTEXTYPE_ALL) {
		glVertexAttribPointer(Mesh::LAYOUTINDEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(Mesh::LAYOUTINDEX_POSITION);
		//		link normal
		glVertexAttribPointer(Mesh::LAYOUTINDEX_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(Mesh::LAYOUTINDEX_NORMAL);
		//		link color(RGBA)
		glVertexAttribPointer(Mesh::LAYOUTINDEX_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glEnableVertexAttribArray(Mesh::LAYOUTINDEX_COLOR);
		//		link texCoord
		glVertexAttribPointer(Mesh::LAYOUTINDEX_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
		glEnableVertexAttribArray(Mesh::LAYOUTINDEX_TEXCOORD);
		//		link tangent
		glVertexAttribPointer(Mesh::LAYOUTINDEX_TANGENT, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
		glEnableVertexAttribArray(Mesh::LAYOUTINDEX_TANGENT);
	}


	// unbind VAO
	// NOTICE: Unbind VAO before EBO and VBO
	glBindVertexArray(0);

	//offsetof(Vertex, normal);
	//		unbind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//		unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Mesh::Load(std::vector <Vertex>& vertices, unsigned int vertexType) {
	Mesh::vertices = std::vector <Vertex>(vertices.begin(), vertices.end());
	init();
}
void Mesh::Load(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, unsigned int vertexType) {
	Mesh::vertices = std::vector <Vertex>(vertices.begin(), vertices.end());
	Mesh::indices = std::vector <GLuint>(indices.begin(), indices.end());

	init();
}

// draw mesh
void Mesh::Draw(GLenum mode) {
	// use vao
	glBindVertexArray(Mesh::vao_ID);
	// draw by EBO
	glDrawElements(mode, Mesh::indices.size(), GL_UNSIGNED_INT, 0);
	// unbind vao
	glBindVertexArray(0);
}
void Mesh::DrawArray(GLenum mode) {
	// use vao
	glBindVertexArray(Mesh::vao_ID);
	// draw by VBO
	glDrawArrays(mode, 0, Mesh::vertices.size());
	// unbind vao
	glBindVertexArray(0);
}

// delete mesh
void Mesh::Delete() {
	if(Mesh::vao_ID != 0)
		glDeleteVertexArrays(1, &(Mesh::vao_ID));
	Mesh::vao_ID = 0;

	if (Mesh::vbo_ID != 0)
		glDeleteBuffers(1, &(Mesh::vbo_ID));
	Mesh::vbo_ID = 0;

	if (Mesh::ebo_ID != 0)
		glDeleteBuffers(1, &(Mesh::ebo_ID));
	Mesh::ebo_ID = 0;
}


// Basic shapes ///////////////////////////////////////////////////////////////////////////////
void basicMesh_Plane(Mesh* mesh, glm::vec2 scale) {
	// generate a x-y plane between -1.0f and 1.0f at z=0.0f

	if (mesh == NULL) return;

	std::vector<Vertex> positions;
	std::vector<GLuint> indices;

	positions.resize(4);
	indices.resize(6);

	positions[0].position = glm::vec3(-1.0f*scale.x, 1.0f*scale.y, 0.0f);
	positions[1].position = glm::vec3( 1.0f*scale.x, 1.0f*scale.y, 0.0f);
	positions[2].position = glm::vec3( 1.0f*scale.x,-1.0f*scale.y, 0.0f);
	positions[3].position = glm::vec3(-1.0f*scale.x,-1.0f*scale.y, 0.0f);

	positions[0].normal = glm::vec3(0.0f, 0.0f,-1.0f);
	positions[1].normal = glm::vec3(0.0f, 0.0f,-1.0f);
	positions[2].normal = glm::vec3(0.0f, 0.0f,-1.0f);
	positions[3].normal = glm::vec3(0.0f, 0.0f,-1.0f);

	positions[0].texCoord = glm::vec2(0.0f, 1.0f);
	positions[1].texCoord = glm::vec2(1.0f, 1.0f);
	positions[2].texCoord = glm::vec2(1.0f, 0.0f);
	positions[3].texCoord = glm::vec2(0.0f, 0.0f);

	positions[0].tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	positions[1].tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	positions[2].tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	positions[3].tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	positions[0].color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	positions[1].color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	positions[2].color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	positions[3].color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);

	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 3;
	indices[4] = 2;
	indices[5] = 0;

	mesh->Load(positions, indices);
}