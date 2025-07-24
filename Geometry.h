#pragma once
#define GLM_ENABLE_EXPERIMENTAL 

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "GPUProgram.h"
#include <vector>

template<class T>
class Geometry {
	unsigned int vao, vbo;
protected:
	std::vector<T> vtx;
public:

	unsigned int getVAO() const {
		return vao;
	}

	unsigned int getVBO() const {
		return vbo;
	}

	Geometry() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);
		int nf = std::min((int)(sizeof(T) / sizeof(float)), 4);
		glVertexAttribPointer(0, nf, GL_FLOAT, GL_FALSE, 0, NULL);
	}
	std::vector<T>& Vtx() { return vtx; }
	void updateGPU() {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vtx.size() * sizeof(T), &vtx[0], GL_DYNAMIC_DRAW);
	}
	void Bind() { glBindVertexArray(vao); glBindBuffer(GL_ARRAY_BUFFER, vbo); }

	void Draw(GLenum type) {
		if (!vtx.empty()) {
			glBindVertexArray(vao);
			glDrawArrays(type, 0, static_cast<GLsizei>(vtx.size()));
		}
	}

	virtual ~Geometry() {
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
};
