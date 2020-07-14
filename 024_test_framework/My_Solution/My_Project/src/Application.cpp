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

#include "tests/TestClearColor.h"


int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // activate debugging
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    // create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // initiating GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW could not be initialized!" << std::endl;
    }

    {
        // error handling
#ifdef DEBUG
		glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(error_occurred_gl, NULL);
#endif

        std::cout << glGetString(GL_VERSION) << std::endl;

        // disable or enable blending
    	glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        Renderer renderer;

    	// initiate ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
    	// set glsl version?????
        ImGui_ImplOpenGL3_Init("#version 330");

        test::TestClearColor test;
    	
        while (!glfwWindowShouldClose(window))
        {
            renderer.clear();

            test.on_update(0.0f);
            test.on_render();

            // new ImGui frame
        	ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            test.on_imgui_render();
        	// render ImGui
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        	
        	glfwSwapBuffers(window);

            glfwPollEvents();
        }

    }

    // shutdown ImGui
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();
	
    glfwTerminate();
    return 0;
}
