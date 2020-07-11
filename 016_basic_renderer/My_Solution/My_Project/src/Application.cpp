// has to be included first
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RGBAColor.h"


int main(void)
{
    GLFWwindow* window;

    // initialize the library
    if (!glfwInit())
        return -1;

	// OpenGL version 4.5
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	// select the core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef DEBUG
    // activate debugging
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

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

	//               <- syncs frame rate with monitors refresh rate
    glfwSwapInterval(1);

    // initiating GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW could not be initialized!" << std::endl;
    }

    {
        ////////////////////
        // error handling //
        ////////////////////

#ifdef DEBUG
    // enable error output
        glEnable(GL_DEBUG_OUTPUT);
        //                                        <- user supplied pointer -> passed on each invocation of callback
        glDebugMessageCallback(error_occurred_gl, NULL);
#endif

        std::cout << glGetString(GL_VERSION) << std::endl;

    	////////////////////////////////////////////////////////////////
    	// create vertex buffer & vertex array & vertex buffer layout //
    	////////////////////////////////////////////////////////////////

        float positions[2 * 4] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,

            -0.5f, 0.5f,
        };

        // create vertex array (used to be vao)
    	VertexArray va;
    	// create vertex buffer with predefined data (positions)
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    	// create vertex buffer layout
        VertexBufferLayout layout;
    	// new attribute (x-y-position with two floats)
        layout.push<float>(2);
    	// add this buffer with this layout to this particular vao
        va.add_buffer(vb, layout);

        /////////////////////////
        // create index buffer //
        /////////////////////////
        
        // one row for each triangle to draw
        // each entry resembles the index of the vertex that needs to be drawn
        unsigned int indices[3 * 2] = {
            0, 1, 2,
            2, 3, 0
        };

        IndexBuffer ib(indices, 6);

        /////////////////////////////////
        // shader definition & uniform //
        /////////////////////////////////

        // default root directory for debugging is the working directory
        Shader shader("res/shaders/Basic.shader");
        shader.bind();
    	//                    <- defined in the fragment shader
        shader.set_uniform_4f("u_color", 0.0f, 0.333f, 0.667f, 1.0f);

        RGBAColor rectangle_color = RGBAColor(0.0f, 0.333f, 0.667f, 1.0f);

        ////////////////
        // games loop //
        ////////////////

        // unbind everything
        va.unbind();
        vb.unbind();
        ib.unbind();
        shader.unbind();

        Renderer renderer;

        // loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            renderer.clear();
            rectangle_color.rotate_color(shader, 0.01f, 0.001f, 0.0001f, 0.0f);
        	renderer.draw(va, ib, shader);

            //             <- mode
            //                           <- count = amount of indices being drawn
            //                              <- type of the data in the index buffer
            //                                               <- pointer to ibo, ibo is bound -> nullptr
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            // swap front and back buffers
            glfwSwapBuffers(window);

            // poll for and process events
            glfwPollEvents();
        }

		// shader deletes itself when the scope ends
    }
	
    glfwTerminate();
    return 0;
}

/*
 * Vertex Array Objects (VAOs) have to be created (the compatibility profile automatically does this)
 *
 * two options:
 *  a) one global VAO
 *      might be faster (should be tested <- environment dependent)
 *  b) one VAO for each unique piece of geometry
 *      the specification says -> use them
 */
