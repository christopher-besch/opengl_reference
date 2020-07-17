#include "SandboxLayer.h"

using namespace GLCore;
using namespace GLCore::Utils;

SandboxLayer::SandboxLayer()
	: m_CameraController(16.0f / 9.0f)
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
	EnableGLDebugging();

	m_Shader = std::unique_ptr<Shader>(Shader::FromGLSLTextFiles(
		"assets/shader.glsl.vert",
		"assets/shader.glsl.frag"
	));

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	float vertices[] = {
		// <- position
		//                       <- color
		-1.5, -0.5, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f,
		-0.5, -0.5, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f,
		-0.5,  0.5, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f,
		-1.5,  0.5, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f,

		 0.5, -0.5, 0.0f, 0.18f, 1.0f, 0.93f, 1.0f,
		 1.5, -0.5, 0.0f, 0.18f, 1.0f, 0.93f, 1.0f,
		 1.5,  0.5, 0.0f, 0.18f, 1.0f, 0.93f, 1.0f,
		 0.5,  0.5, 0.0f, 0.18f, 1.0f, 0.93f, 1.0f
	};

	glCreateVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);

	glCreateBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(m_QuadVB, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void*)0);

	glEnableVertexArrayAttrib(m_QuadVB, 1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void*)(3 * sizeof(float)));

	uint32_t indices[] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};

	glCreateBuffers(1, &m_QuadIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void SandboxLayer::OnDetach()
{
	// Shutdown here
}

void SandboxLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);
}

void SandboxLayer::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_Shader->GetRendererID());

	auto vp = m_CameraController.GetCamera().GetViewProjectionMatrix();

	glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetRendererID(), "u_ViewProj"), 1, GL_FALSE, &vp[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetRendererID(), "u_Transform"), 1, GL_FALSE, &glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))[0][0]);

	glBindVertexArray(m_QuadVA);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
}

void SandboxLayer::OnImGuiRender()
{
	// ImGui here
}
