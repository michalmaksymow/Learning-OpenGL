#pragma once

#include "Test.h"

TEST_BEGIN
class TestClearColor : public Test {

public:
	TestClearColor();
	~TestClearColor();

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnImGuiRender() override;

private:
	float m_ClearColor[4];
};
TEST_END