#include <vector>

#include "imgui/imgui.h"

#include "Test.h"


namespace test
{
	TestMenu::TestMenu(Test*& current_test_pointer)
		: m_current_test(current_test_pointer)
	{
		
	}

	void TestMenu::on_imgui_render()
	{
		for (auto& test : m_tests)
		{
			if (ImGui::Button(test.first.c_str()))
				m_current_test = test.second();
		}
	}
}
