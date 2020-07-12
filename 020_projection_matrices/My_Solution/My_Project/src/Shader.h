#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
// hashtable
#include <unordered_map>

struct ShaderProgramSource
{
	std::string vertex_shader_source;
	std::string fragment_shader_source;
};


class Shader
{
private:
	std::string m_filepath;
	unsigned int m_renderer_id;
	std::unordered_map<std::string, int> m_uniform_location_cache;
public:
	Shader (const std::string& filepath);
	~Shader();

	// actually use but every other class uses bind
	void bind();
	void unbind();

	// set uniforms
	void set_uniform_1i(const std::string& name, int value);
	void set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3);
	void set_uniform_mat4f(const std::string& name, const glm::mat4& matrix);
private:
	ShaderProgramSource parse_shader(const std::string& file_path);
	unsigned int compile_shader(unsigned int type, const std::string& source);
	unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader);
	unsigned int get_uniform_location(const std::string& name);
};
