#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "test/TestClearColor.h"
#include "test/TestMenu.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the GLFW library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const char* glsl_version = "#version 130";

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Synchronize with v-sync */
    glfwSwapInterval(1);

    /* Initialize the GLEW library */
    if (glewInit() != GLEW_OK)
        return -1;

    std::cout << glGetString(GL_VERSION) << std::endl;


    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    Renderer renderer;

    ImGui::CreateContext();
    IMGUI_CHECKVERSION();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsDark();

    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestClearColor>("Clear Color");

    test::TestClearColor test;

    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (currentTest)
        {
            currentTest->OnUpdate(0.0f);
            ImGui::Begin("Test");
            if (currentTest != testMenu && ImGui::Button("<-"))
            {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->OnImGuiRender();
            currentTest->OnRender();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    if (currentTest != testMenu)
    {
        delete testMenu;
    }
    delete currentTest;
    
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return EXIT_SUCCESS;
}