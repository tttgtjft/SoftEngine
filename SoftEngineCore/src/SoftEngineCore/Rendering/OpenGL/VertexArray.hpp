#pragma once

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace SoftEngine {

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray&) = delete;
		VertexArray(VertexArray&& vertex_array) noexcept;
		VertexArray& operator=(VertexArray&& vertex_array) noexcept;

		void add_vertex_buffer(const VertexBuffer& vertex_buffer);
		void set_index_buffer(const IndexBuffer& index_buffer);
		void bind() const;
		static void unbind();
		const size_t get_indices_count() const { return m_count; }

	private:
		unsigned int m_id = 0;
		unsigned int m_elements_count = 0;
		size_t m_count = 0;
	};

}