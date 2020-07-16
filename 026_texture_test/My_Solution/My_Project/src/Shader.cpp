#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath)
	: m_filepath(filepath), m_renderer_id(0)
{
	ShaderProgramSource source = parse_shader(filepath);
	m_renderer_id = create_shader(source.vertex_shader_source, source.fragment_shader_source);
}

Shader::~Shader()
{
	glDeleteProgram(m_renderer_id);
}

ShaderProgramSource Shader::parse_shader(const std::string& file_path)
{
	std::fstream stream(file_path);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream string_stream[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		//       <- npos if not found, position if found
		if (line.find("shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				// set mode to vertex
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				// set mode to fragment
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			// save line
			string_stream[(int)type] << line << std::endl;
		}
	}

	return { string_stream[0].str(), string_stream[1].str() };
}

unsigned int Shader::compile_shader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	// same as &source[0]
	//                 <- how many source codes
	//                    <- pointer to the pointer
	//                          <- length, NULL -> NULL-terminated string
	glShaderSource(id, 1, &src, nullptr);
	// actually compile
	glCompileShader(id);

	// error handling
	int result;
	//         <- specifying an integer
	//          <- requests a vector -> a pointer
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		// char message[length]; <- doesn't work <- length is unknown at compile time
		//                     <- allocates on the stack
		char* message = (char*)alloca(length * sizeof(char));
		//                             <- can return the length of the log (excluding NULL-termination character)
		glGetShaderInfoLog(id, length, nullptr, message);

		// print error
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
			" shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
	// same as GLuint
	unsigned int program = glCreateProgram();
	unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
	unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

	// linking the shader
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
#ifdef DEBUG
	glValidateProgram(program);
#endif

	// delete intermediates
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

void Shader::bind()
{
	glUseProgram(m_renderer_id);
}

void Shader::unbind()
{
	glUseProgram(0);
}

void Shader::set_uniform_1i(const std::string& name, int value)
{
	glUniform1i(get_uniform_location(name), value);
}

void Shader::set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(get_uniform_location(name), v0, v1, v2, v3);
}

void Shader::set_uniform_mat4f(const std::string& name, const glm::mat4& matrix)
{
	//               <- v stands for array -> an array has to be parsed
	//                                             <- how many matrices
	//												  <- needs to be transposed? (glm stores matrices in column major -> not necessary)
	//												            <- pointer to first element
	glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &matrix[0][0]);
}

unsigned Shader::get_uniform_location(const std::string& name)
{
	if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
	{
		return m_uniform_location_cache[name];
	}
	
	int location = glGetUniformLocation(m_renderer_id, name.c_str());
	if (location == -1)
	{
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	} else
	{
		m_uniform_location_cache[name] = location;
	}
	return location;
}

/*
 * it should be glDeleteProgram(), instead of glDeleteShader()
 */
