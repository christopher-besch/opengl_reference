#pragma once

#include "Test.h"

namespace test
{

	class TestRectangleColor : public Test
	{
	private:
		float m_rectangle_color[4];
	public:
		TestRectangleColor();
		~TestRectangleColor();

		void on_update(float delta_time) override;
		void on_render() override;
		void on_imgui_render() override;
	};
	
}
