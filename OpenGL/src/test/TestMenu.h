#pragma once

#include "Test.h"

#include <vector>
#include <string>
#include <functional>
#include <iostream>

TEST_BEGIN
class TestMenu : public Test {

public:
	TestMenu(Test*& currentTestPointer);

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnImGuiRender() override;

	template<typename T>
	void RegisterTest(const std::string& name)
	{
		std::cout << "Registering test: " << name << std::endl;
		m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
	}

private:
	Test*& m_CurrentTest;
	std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;;
};
TEST_END
