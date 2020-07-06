// has to be included first
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // opengl rendering context is created here
	/* Make the window's context current */
    glfwMakeContextCurrent(window);

    // initiating GLEW
	if (glewInit() != GLEW_OK)
	{
        std::cout << "GLEW could not be initialized!" << std::endl;
	}

    std::cout << glGetString(GL_VERSION) << std::endl;

    // give OpenGL the data

    // vertices of a triangle
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

	// create buffer and its id
	unsigned int buffer;
    glGenBuffers(1, &buffer);
	// selecting the buffer with its id
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//                            <- size of the buffer in bytes
	//                                               <- source
	//                                                             <- STREAM -> modified once, used few times; STATIC -> modified once, used often, DYNAMIC -> modified repeatedly, used often
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	///////////////////////////////////
	// new                           //
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
	
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // games loop
	/* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // using the currently bound buffer
    	//                         <- point in the array at which to start
    	//                            <- number of vertices              
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

/*
 * vertex buffer:
 *  buffer in V-RAM
 *  can be used in the draw call (by the CPU)
 *  -> executed by the shader (running on the GPU)
 *
 * OpenGL is a state machine -> can't be treated as an object -> functions don't get the necessary data parsed but OpenGL already known everything it needs
 *
 * docs.gl is a good reference site for OpenGL
 */
