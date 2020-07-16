#pragma once

#include <memory>

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

namespace test
{

	class TestTexture2d : public Test
	{
	private:
		glm::vec3 m_translation_a;
		glm::vec3 m_translation_b;

		std::unique_ptr<VertexArray> m_vao;
		std::unique_ptr<VertexBuffer> m_vbo;
		std::unique_ptr<IndexBuffer> m_ibo;
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<Texture> m_texture;

		glm::mat4 m_proj;
		glm::mat4 m_view;
		
	public:
		TestTexture2d();
		~TestTexture2d();

		void on_update(float delta_time) override;
		void on_render() override;
		void on_imgui_render() override;
	};

}
