#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray {

private:
	uint32_t m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};
