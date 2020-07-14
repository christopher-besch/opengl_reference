// has to be included first
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include <iostream>
#include <fstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RGBAColor.h"
#include "Texture.h"


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

    	//////////////
    	// blending //
    	//////////////

        // disable or enable blending
    	glEnable(GL_BLEND);
        //          <- multiplier for the source RGBA factor (default GL_ONE)
        //                        <- multiplier for the destination RGBA factor (default GL_ZERO)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    	//                 <- how to combine source and destination (default GL_FUNC_ADD)
    	// glBlendEquation(GL_FUNC_ADD)

    	////////////////////////////////////////////////////////////////
    	// create vertex buffer & vertex array & vertex buffer layout //
    	////////////////////////////////////////////////////////////////

        float positions[4 * 4] = {
        	// <- position
        	//            <- texture coordinates
            -50.0f, -50.0f, 0.0f, 0.0f, // bottom left
             50.0f, -50.0f, 1.0f, 0.0f, // bottom right
             50.0f,  50.0f, 1.0f, 1.0f, // top right
            -50.0f,  50.0f, 0.0f, 1.0f  // top left
        };

        // create vertex array (used to be vao)
    	VertexArray va;
    	// create vertex buffer with predefined data (positions)
    	// four vertices with two floats for the position and two floats for the texture coordinates
        VertexBuffer vb(positions, 4 * (2 * sizeof(float) + 2 * sizeof(float)));
    	// create vertex buffer layout
        VertexBufferLayout layout;
    	// new attribute (x-y-position with two floats)
        layout.push<float>(2);
    	// new attribute (texture coordinates)
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

        /////////////////////////////////
        // MVP (model view projection) //
        /////////////////////////////////

        // model matrix : defines position, rotation and scale of the vertices of the model in the world.
        // view matrix : defines position and orientation of the "camera".
        // projection matrix : Maps what the "camera" sees to NDC, taking care of aspect ratio and perspective.

    	// projection matrix
        // left to right 960, bottom to top 540 -> 4:3
        //                    <- like a orthographic camera, not perspective
        //                          <- those four values define the position of the left, right bottom and top side of the normalized device coordinates
        glm::mat4 proj = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);
    	
    	// view matrix
    	// the "camera" can't be moved but the objects can -> instead of moving the camera to the left, move everything to the right
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    	// model matrix
    	// move the model up a bit (will be overridden by ImGui)
        glm::vec3 translation_a(0, 100, 0);
    	// move the second model a bit more
        glm::vec3 translation_b(0, 300, 0);

    	///////////////////
    	// texture setup //
    	///////////////////
        
        Texture texture("res/textures/lambda.png");
    	//           <- this has to
        texture.bind(0);
        //                                 <- match this
        shader.set_uniform_1i("u_texture", 0);

        // unbind everything
        va.unbind();
        vb.unbind();
        ib.unbind();
        shader.unbind();

        Renderer renderer;

    	////////////////////
    	// initiate ImGui //
    	////////////////////

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

        // loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            renderer.clear();

            // new ImGui frame
        	ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // model matrix for the first rectangle
            glm::mat4 model_a = glm::translate(glm::mat4(1.0f), translation_a);
            glm::mat4 model_b = glm::translate(glm::mat4(1.0f), translation_b);
            // set MVP
            glm::mat4 mvp = proj * view * model_a;
            shader.set_uniform_mat4f("u_mvp", mvp);
        	// replaced by the texture
            // rectangle_color.rotate_color(shader, 0.01f, 0.001f, 0.0001f, 0.0f);
            renderer.draw(va, ib, shader);

            // set MVP for the second draw call
            mvp = proj * view * model_b;
            shader.set_uniform_mat4f("u_mvp", mvp);
            renderer.draw(va, ib, shader);

            //             <- mode
            //                           <- count = amount of indices being drawn
            //                              <- type of the data in the index buffer
            //                                               <- pointer to ibo, ibo is bound -> nullptr
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        	///////////
            // ImGui //
            ///////////
        	
            {
                //                                 <- pointer to first element
                ImGui::SliderFloat3("Translation A", &translation_a.x, -700.0f, 700.0f);
                ImGui::SliderFloat3("Translation B", &translation_b.x, -700.0f, 700.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

        	// render ImGui
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        	
        	// swap front and back buffers
            glfwSwapBuffers(window);

            // poll for and process events
            glfwPollEvents();
        }

		// shader deletes itself when the scope ends
    }

    // shutdown ImGui
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();
	
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
 *
 *
 *
 * blending comes in play when using transparent textures
 * blending determines how to combine the output (=source, the color outputted by the fragment shader) color with what is already in the target buffer (=destination, the buffer the fragment shader is drawing to)
 *
 * projection matrices are part of the transformation pipeline (project 3d world onto 2d screen)
 *the transformation with a transformation matrix converts vertex positions into normalized device coordinates
 * normalized device coordinates:
 * left side   -1
 * right side   1
 * bottom side -1
 * top side     1
 *
 * TRS:
 *  translation: position of the object
 *  rotation: rotation of the object
 *  scale: scale of the object
 */
