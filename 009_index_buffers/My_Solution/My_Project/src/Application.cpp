// has to be included first
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


struct ShaderProgramSource
{
    std::string vertex_shader_source;
    std::string fragment_shader_source;
};


static ShaderProgramSource parse_shader(const std::string& file_path)
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
			} else if (line.find("fragment") != std::string::npos)
			{
                // set mode to fragment
                type = ShaderType::FRAGMENT;
			}
		} else
		{
			// save line
            string_stream[(int)type] << line << std::endl;
		}
	}

    return { string_stream[0].str(), string_stream[1].str()  };
}


static unsigned int compile_shader(unsigned int type, const std::string& source)
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


static unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
    // same as GLuint
	unsigned int program = glCreateProgram();
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

	// "compiling" the shader
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

	// delete intermediates
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


int main(void)
{
    GLFWwindow* window;

    // initialize the library
    if (!glfwInit())
        return -1;

    // create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // opengl rendering context is created here
	// make the window's context current
    glfwMakeContextCurrent(window);

    // initiating GLEW
	if (glewInit() != GLEW_OK)
	{
        std::cout << "GLEW could not be initialized!" << std::endl;
	}

    std::cout << glGetString(GL_VERSION) << std::endl;

	//////////////////////////
    // create vertex buffer //
	//////////////////////////

    float positions[2 * 4] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f, 0.5f,

        -0.5f, 0.5f,
    };

	// one row for each triangle to draw
	// each entry resembles the index of the vertex that needs to be drawn
    unsigned int indices[3 * 2] = {
        0, 1, 2,
    	2, 3, 0
    };

	// create vertex buffer and its id
	unsigned int buffer;
    glGenBuffers(1, &buffer);
	// selecting the buffer with its id
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//                            <- size of the buffer in bytes
	//                                                   <- source
	//                                                                 <- STREAM -> modified once, used few times; STATIC -> modified once, used often, DYNAMIC -> modified repeatedly, used often
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	///////////////////////////////////
	// define the layout of the data //
	///////////////////////////////////
	
	glEnableVertexAttribArray(0);
    // each vertex can have multiple attributes, one of which can be the position
    // define the layout of the data
	// defining 1 attribute, the position
	//                    <- index: index of this attribute in the each vertex, first attribute, second, third...
	//                       <- size: how many values are representing this attribute; 1, 2, 3, 4 are possible
	//                          <- type of the values in this attribute
	//                                    <- normalized: when true converts int (0-255) to float (0.0f - 1.0f)
    //                                              <- stride: amount of bytes between each vertex start
    //                                                                 <- pointer: inside the space of a vertex, the start of this attribute, first attribute -> 0, second attribute -> >0
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);

	/////////////////////////
    // create index buffer //
    /////////////////////////
    
	// index buffer object
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//                                           <- has to be unsigned
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	///////////////////////
    // shader definition //
    ///////////////////////

    // default root directory for debugging is the working directory
    ShaderProgramSource source = parse_shader("res/shaders/Basic.shader");

    unsigned int shader = create_shader(source.vertex_shader_source, source.fragment_shader_source);
    glUseProgram(shader);

    // games loop
	// loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        //                 <- mode
        //                           <- count = amount of indices being drawn
        //                              <- type of the data in the index buffer
    	//                                               <- pointer to ibo, ibo is bound -> nullptr
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
