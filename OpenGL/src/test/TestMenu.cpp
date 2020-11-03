#include "TestMenu.h"

#include <imgui/imgui.h>

TEST_BEGIN
TestMenu::TestMenu(Test*& currentTestPointer)
	: m_CurrentTest(currentTestPointer)
{
}

void TestMenu::OnUpdate(float deltaTime)
{
}

void TestMenu::OnRender()
{
}

void TestMenu::OnImGuiRender()
{
	for (auto& test : m_Tests)
	{
		if (ImGui::Button(test.first.c_str()))
			m_CurrentTest = test.second();
	}
}
TEST_END


