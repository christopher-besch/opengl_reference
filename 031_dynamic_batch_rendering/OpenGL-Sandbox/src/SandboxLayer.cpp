#include "SandboxLayer.h"

#include "stb_image/stb_image.h"

using namespace GLCore;
using namespace GLCore::Utils;

SandboxLayer::SandboxLayer()
	: m_CameraController(16.0f / 9.0f)
{
}

SandboxLayer::~SandboxLayer()
{
}

struct Vertex
{
	float Position[3];
	float Color[4];
	float TexCoords[2];
	float TexID;
};

static GLuint LoadTexture(const std::string& path)
{
	int w, h, bits;

	stbi_set_flip_vertically_on_load(1);
	auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb);
	GLuint textureID;
	glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	stbi_image_free(pixels);
	return textureID;
}

void SandboxLayer::OnAttach()
{
	EnableGLDebugging();

	m_Shader = std::unique_ptr<Shader>(Shader::FromGLSLTextFiles(
		"assets/shader.glsl.vert",
		"assets/shader.glsl.frag"
	));

	glUseProgram(m_Shader->GetRendererID());
	auto loc = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
	int samplers[2] = { 0, 1 };
	glUniform1iv(loc, 2, samplers);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	/*float vertices[2*4*(3+4+2+1)] = {
		// <- position
		//                <- color
		//                                          <- texture coordinates
		//                                                      <- texture index
		-1.5, -0.5, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f,	0.0f,
		-0.5, -0.5, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f,	0.0f,
		-0.5,  0.5, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f,	0.0f,
		-1.5,  0.5, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f,	0.0f, 1.0f,	0.0f,

		 0.5, -0.5, 0.0f, 0.18f, 1.0f, 0.93f, 1.0f, 0.0f, 0.0f,	1.0f,
		 1.5, -0.5, 0.0f, 0.18f, 1.0f, 0.93f, 1.0f, 1.0f, 0.0f,	1.0f,
		 1.5,  0.5, 0.0f, 0.18f, 1.0f, 0.93f, 1.0f, 1.0f, 1.0f,	1.0f,
		 0.5,  0.5, 0.0f, 0.18f, 1.0f, 0.93f, 1.0f,	0.0f, 1.0f, 1.0f
	};*/

	glCreateVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);

	glCreateBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	//                            <- can store 1000 of those vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 1000, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexArrayAttrib(m_QuadVB, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));

	glEnableVertexArrayAttrib(m_QuadVB, 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));

	glEnableVertexArrayAttrib(m_QuadVB, 2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));

	glEnableVertexArrayAttrib(m_QuadVB, 3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));

	uint32_t indices[] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};

	glCreateBuffers(1, &m_QuadIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	m_LambdaTex = LoadTexture("assets/lambda.png");
	m_CubeTex = LoadTexture("assets/companion_cube.png");
}

void SandboxLayer::OnDetach()
{
	// Shutdown here
}

void SandboxLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);
}

std::array<Vertex, 4> CreateQuad(float x, float y, float textureID)
{
	float size = 1.0f;
	
	Vertex v0 = {
		{ x, y, 0.0f },
		{ 0.18f, 0.6f, 0.96f, 1.0f },
		{ 0.0f, 0.0f },
		textureID
	};

	Vertex v1 = {
		{ x + size, y, 0.0f },
		{ 0.18f, 0.6f, 0.96f, 1.0f },
		{ 1.0f, 0.0f },
		textureID
	};

	Vertex v2 = {
		{ x + size, y + size, 0.0f },
		{ 0.18f, 0.6f, 0.96f, 1.0f },
		{ 1.0f, 1.0f },
		textureID
	};

	Vertex v3 = {
		{ x, y + size, 0.0f },
		{ 0.18f, 0.6f, 0.96f, 1.0f },
		{ 0.0f, 1.0f },
		textureID
	};

	return { v0, v1, v2, v3 };
}

void SandboxLayer::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	// set dynamic vertex buffer
	auto q0 = CreateQuad(m_QuadPosition[0], m_QuadPosition[1], 0.0f);
	auto q1 = CreateQuad(0.5f, -0.5f, 1.0f);

	Vertex vertices[2 * 4];
	memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
	memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));
	
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_Shader->GetRendererID());
	glBindTextureUnit(0, m_LambdaTex);
	glBindTextureUnit(1, m_CubeTex);

	auto vp = m_CameraController.GetCamera().GetViewProjectionMatrix();

	glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetRendererID(), "u_ViewProj"), 1, GL_FALSE, &vp[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetRendererID(), "u_Transform"), 1, GL_FALSE, &glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))[0][0]);

	glBindVertexArray(m_QuadVA);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
}

void SandboxLayer::OnImGuiRender()
{
	ImGui::Begin("Controls");
	ImGui::DragFloat2("Quad Position", m_QuadPosition, 0.1f);
	ImGui::End();
}
