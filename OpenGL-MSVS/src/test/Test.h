#pragma once

#define TEST_BEGIN namespace test {
#define TEST_END }


TEST_BEGIN
class Test {

public:
	Test() {}
	virtual ~Test() {}

	virtual void OnUpdate(float deltaTime) {}
	virtual void OnRender() {}
	virtual void OnImGuiRender() {}
};
TEST_END