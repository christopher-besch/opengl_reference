// has to be included first
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
    // give OpenGL the data //
	//////////////////////////

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
	// loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        // using the currently bound buffer
    	//                         <- point in the array at which to start
    	//                            <- number of vertices              
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

/*
 * rendering pipeline -> data to rendered image on screen
 *
 * vertex shader:
 *  gets called first in pipeline (when draw command is issued)
 *  called for each vertex
 *  -> tell OpenGL where the vertex gos (on the screen)
 *  takes in vertex attributes
 * fragment shader (pixel shader):
 *  gets called second in pipeline (many things in between)
 *  gets called for each pixel
 *  decides color for each pixel
 * 
 * there are others (like tessellation, geometry shaders, compute shaders)
 */
