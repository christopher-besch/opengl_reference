#pragma once

#include <functional>
#include <iostream>

#include "Renderer.h"


namespace test
{

	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void on_update(float delta_time) {}
		virtual void on_render() {}
		virtual void on_imgui_render() {}
	};


	class TestMenu : public Test
	{
	private:
		Test*& m_current_test;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_tests;
	public:
		TestMenu(Test*& current_test_pointer);

		void on_imgui_render() override;

		template<typename T>
		void register_test(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;
			
			m_tests.push_back(std::make_pair(name, []() { return new T; }));
		}
	};
	
}
