#include "TestTexture2D.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

namespace test
{
	
	TestTexture2d::TestTexture2d()
		: m_translation_a(0, 100, 0), m_translation_b(0, 300, 0),
		m_proj(glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f)),
        m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
        // vertex buffer & vertex array
		float positions[4 * 4] = {
            -50.0f, -50.0f, 0.0f, 0.0f, // bottom left
             50.0f, -50.0f, 1.0f, 0.0f, // bottom right
             50.0f,  50.0f, 1.0f, 1.0f, // top right
            -50.0f,  50.0f, 0.0f, 1.0f  // top left
        };
        m_vao = std::make_unique<VertexArray>();
        m_vbo = std::make_unique<VertexBuffer>(positions, 4 * (2 * sizeof(float) + 2 * sizeof(float)));
        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        m_vao->add_buffer(*m_vbo, layout);

        // create index buffer
        unsigned int indices[3 * 2] = {
            0, 1, 2,
            2, 3, 0
        };
        m_ibo = std::make_unique<IndexBuffer>(indices, 6);

        // shader definition
        m_shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_shader->bind();

        // texture setup
        m_texture = std::make_unique<Texture>("res/textures/lambda.png");

	}

	TestTexture2d::~TestTexture2d()
	{
	}

	void TestTexture2d::on_update(float delta_time)
	{
	}

	void TestTexture2d::on_render()
	{
        Renderer renderer;
        m_texture->bind(0);
        m_shader->set_uniform_1i("u_texture", 0);

		glm::mat4 model_a = glm::translate(glm::mat4(1.0f), m_translation_a);
        glm::mat4 model_b = glm::translate(glm::mat4(1.0f), m_translation_b);

        {
            glm::mat4 mvp = m_proj * m_view * model_a;
            m_shader->set_uniform_mat4f("u_mvp", mvp);
            renderer.draw(*m_vao, *m_ibo, *m_shader);
        }
        {
            glm::mat4 mvp = m_proj * m_view * model_b;
            m_shader->set_uniform_mat4f("u_mvp", mvp);
            renderer.draw(*m_vao, *m_ibo, *m_shader);
        }

	}

	void TestTexture2d::on_imgui_render()
	{
        ImGui::SliderFloat3("Translation A", &m_translation_a.x, -700.0f, 700.0f);
        ImGui::SliderFloat3("Translation B", &m_translation_b.x, -700.0f, 700.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	
}
