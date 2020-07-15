#include "TestRectangleColor.h"
#include <VertexArray.h>
#include <IndexBuffer.h>
#include <Shader.h>
#include <VertexBufferLayout.h>

namespace test
{
	TestRectangleColor::TestRectangleColor()
		: m_rectangle_color{ 0.2f, 0.7f, 0.1f, 1.0f }
	{
        ////////////////////////////////////////////////////////////////
        // create vertex buffer & vertex array & vertex buffer layout //
        ////////////////////////////////////////////////////////////////

        float positions[2 * 4] = {
            // <- position
            -50.0f, -50.0f, // bottom left
             50.0f, -50.0f, // bottom right
             50.0f,  50.0f, // top right
            -50.0f,  50.0f  // top left
        };

        // create vertex array (used to be vao)
        VertexArray va;
        // create vertex buffer with predefined data (positions)
        VertexBuffer vb(positions, 4 * (2 * sizeof(float)));
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
        Shader shader("res/shaders/TestRectangleColor.shader");
        shader.bind();
        //                    <- defined in the fragment shader
        shader.set_uniform_4f("u_color", m_rectangle_color[0], m_rectangle_color[1], m_rectangle_color[2], m_rectangle_color[3]);
	}

	TestRectangleColor::~TestRectangleColor()
	{
	}

	void TestRectangleColor::on_update(float delta_time)
	{
		
	}

	void TestRectangleColor::on_render()
	{
        renderer.draw(va, ib, shader);
	}

	void TestRectangleColor::on_imgui_render()
	{
	}
}
